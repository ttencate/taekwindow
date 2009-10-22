#include "messages.hpp"
#include "trayicon.hpp"
#include "util.hpp"
#include "config.hpp"
#include "version.h"

#include <windows.h>
#include <tchar.h>

HINSTANCE currentInstance = NULL;
HMODULE dllHandle = NULL;

DWORD (*initProc)(DWORD, DWORD) = NULL;
void (*uninitProc)() = NULL;
void (*applyConfigProc)(DLLConfiguration*) = NULL;

HOOKPROC mouseProc = NULL;
HOOKPROC lowLevelKeyboardProc = NULL;

HHOOK mouseHook = NULL;
HHOOK lowLevelKeyboardHook = NULL;

EXEConfiguration activeExeConfig;

HINSTANCE getCurrentInstance() {
	return currentInstance;
}

/* Loads and initializes the DLL with the hook handlers.
 * Returns true on success.
 */
bool loadDll() {
	dllHandle = LoadLibrary(_T(HOOKS_DLL_FILE) _T(".dll"));
	if (!dllHandle)
		return false;
	return true;
}

/* Unloads the DLL. Returns true on success.
 */
bool unloadDll() {
	return FreeLibrary(dllHandle) != 0;
}

/* Initializes the DLL by passing in the current thread ID.
 * Returns NULL on success, or the thread ID of the thread that was there before us on failure.
 */
DWORD initDll() {
	return (*initProc)(GetCurrentThreadId(), GetCurrentProcessId());
}

/* Uninitializes the DLL by making it forget our thread ID.
 * This way, if the DLL (for some reason) lingers in memory after we've shut down,
 * it will not hold a nonexistent thread ID, and therefore be available when a new instance of
 * the .exe needs it.
 */
void uninitDll() {
	(*uninitProc)();
}

/* Updates the configuration settings in the DLL.
 */
void applyDllConfig(DLLConfiguration *dllconfig) {
	(*applyConfigProc)(dllconfig);
}

/* Initializes the function pointers to functions in the DLL.
 * Returns true on success.
 */
bool findProcs() {
	initProc = (DWORD (*)(DWORD, DWORD))GetProcAddress(dllHandle, "init");
	if (!initProc)
		return false;
	uninitProc = (void (*)())GetProcAddress(dllHandle, "uninit");
	if (!uninitProc)
		return false;
	applyConfigProc = (void (*)(DLLConfiguration*))GetProcAddress(dllHandle, "applyConfig");
	if (!applyConfigProc)
		return false;
	mouseProc = (HOOKPROC)GetProcAddress(dllHandle, "mouseProc");
	if (!mouseProc)
		return false;
	lowLevelKeyboardProc = (HOOKPROC)GetProcAddress(dllHandle, "lowLevelKeyboardProc");
	if (!lowLevelKeyboardProc)
		return false;
	return true;
}

/* Attaches global event hooks.
 * Currently this is only done for mouse events; keyboard events may be added later.
 * Returns true on success.
 */
bool attachHooks() {
	mouseHook = SetWindowsHookEx(WH_MOUSE, mouseProc, dllHandle, NULL);
	if (!mouseHook)
		return false;
	lowLevelKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, lowLevelKeyboardProc, dllHandle, NULL);
	if (!lowLevelKeyboardHook)
		return false;
	return true;
}

/* Detaches previously set hooks.
 * Returns true on success.
 */
bool detachHooks() {
	bool success = true;
	if (!UnhookWindowsHookEx(lowLevelKeyboardHook))
		success = false;
	lowLevelKeyboardHook = NULL;
	if (!UnhookWindowsHookEx(mouseHook))
		success = false;
	mouseHook = NULL;
	return success;
}

bool isEnabled() {
	return (mouseHook && lowLevelKeyboardHook);
}

bool enable() {
	if (isEnabled())
		return true;
	if (!attachHooks())
		return false;
	updateTrayIcon();
	return true;
}

bool disable() {
	if (!isEnabled())
		return true;
	if (!detachHooks())
		return false;
	updateTrayIcon();
	return true;
}

void applyExeConfig(EXEConfiguration *config) {
	activeExeConfig = *config;
	showTrayIcon(config->systemTrayIcon);
}

void applyConfig(DLLConfiguration *dllConfig, EXEConfiguration *exeConfig) {
	applyDllConfig(dllConfig);
	applyExeConfig(exeConfig);
}

void loadAndApplyConfig() {
	DLLConfiguration dllConfig;
	EXEConfiguration exeConfig;

	dllConfig.setDefaults();
	exeConfig.setDefaults();

	loadConfig(&dllConfig, &exeConfig);

	applyConfig(&dllConfig, &exeConfig);
}

/* Runs the message loop until it is time to quit.
 * Returns the desired exit status of the program.
 */
int messageLoop() {
	BOOL getMsgRetVal;
	MSG msg;
	do {
		getMsgRetVal = GetMessage(&msg, NULL, 0, 0);
		if (getMsgRetVal == -1) {
			// error in GetMessage... low-level, panic and abort
			break;
		} else {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	} while (getMsgRetVal);
	if (getMsgRetVal) {
		// GetMessage returned -1: error code.
		return -1;
	} else {
		// Normal exit: return the WPARAM from the WM_QUIT.
		return (int)msg.wParam;
	}
}

/* The main function for the application.
 * Called from entryPoint().
 */
int myMain(HINSTANCE hInstance) {
	currentInstance = hInstance;
	int retVal = -1; // value to be returned eventually, after cleaning up etc.
	// First, load the DLL with the event handlers in it.
	if (!loadDll()) {
		showLastError(NULL, _T("Error loading DLL"));
	} else {
		// DLL loaded, get pointers to the functions in it that we need.
		if (!findProcs()) {
			showLastError(NULL, _T("Error getting handler address"));
		} else {
			// Function pointers acquired, initialize the DLL.
			DWORD prevThreadId = initDll();
			if (prevThreadId) {
				// Somebody has been there before us.
				// Kick that instance in the nuts.
				MessageBox(NULL, _T(APPLICATION_TITLE) _T(" is already running and will now be stopped.\n\nRerun the program it if you want to start it again."), _T("Taekwindow already running"), MB_OK | MB_ICONINFORMATION);
				PostThreadMessage(prevThreadId, WM_QUIT, 0, 0);
			} else {
				// We're the first to initialize the DLL, continue.
				// Load the configuration from the registry.
				loadAndApplyConfig();
				// Attach the event hooks.
				if (!enable()) {
					showLastError(NULL, _T("Error attaching hooks"));
				} else {
					// main message loop
					retVal = messageLoop();
				}
				// Normal exit.
				showTrayIcon(false);
				// Note that calling detachHooks is OK if attachHooks only partly worked.
				detachHooks();
				// Make the DLL forget about our existence.
				uninitDll();
			}
		}
		unloadDll();
	}
	return retVal;
}

/* The actual entry point to the application.
 * This replaces WinMainCRTStartup from the CRT.
 */
int entryPoint() {
	int mainret = 0;
	HINSTANCE hInstance = GetModuleHandle(NULL);
	mainret = myMain(hInstance);
	return mainret;
}
