#include "messages.hpp"
#include "trayicon.hpp"
#include "util.hpp"
#include "config.hpp"
#include "version.h"
#include "dllmain.hpp"
#include "hooks.hpp"

#include <windows.h>
#include <tchar.h>

HINSTANCE currentInstance = NULL;

HHOOK lowLevelMouseHook = NULL;
HHOOK lowLevelKeyboardHook = NULL;

EXEConfiguration activeExeConfig;

HINSTANCE getCurrentInstance() {
	return currentInstance;
}

/* Initializes the DLL by passing in the current thread ID.
 * Returns NULL on success, or the thread ID of the thread that was there before us on failure.
 */
DWORD initDll() {
	return init(GetCurrentThreadId(), GetCurrentProcessId());
}

/* Uninitializes the DLL by making it forget our thread ID.
 * This way, if the DLL (for some reason) lingers in memory after we've shut down,
 * it will not hold a nonexistent thread ID, and therefore be available when a new instance of
 * the .exe needs it.
 */
void uninitDll() {
	uninit();
}

/* Attaches global event hooks.
 * Returns true on success.
 */
bool attachHooks() {
	lowLevelMouseHook = SetWindowsHookEx(WH_MOUSE_LL, lowLevelMouseProc, NULL, NULL);
	if (!lowLevelMouseHook)
		return false;
	lowLevelKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, lowLevelKeyboardProc, NULL, NULL);
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
	if (!UnhookWindowsHookEx(lowLevelMouseHook))
		success = false;
	lowLevelMouseHook = NULL;
	return success;
}

bool isEnabled() {
	return (lowLevelMouseHook && lowLevelKeyboardHook);
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
