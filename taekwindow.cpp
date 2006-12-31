#include <windows.h>
#include <stdio.h>

const LPCSTR INIT_ORDINAL = (LPCSTR)1;
const LPCSTR MOUSEPROC_ORDINAL = (LPCSTR)3;

HMODULE dllHandle = NULL;
HOOKPROC mouseProc = NULL;
DWORD (__stdcall *initProc)(DWORD) = NULL;
HHOOK mouseHook = NULL;

/* Loads and initializes the DLL with the hook handlers.
 * Returns true on success.
 */
bool loadDll() {
	dllHandle = LoadLibrary(L"taekwindowhooks.dll");
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
	return (*initProc)(GetCurrentThreadId());
}

/* Initializes the function pointers to functions in the DLL.
 * Returns true on success.
 */
bool findProcs() {
	initProc = (DWORD (__stdcall*)(DWORD))GetProcAddress(dllHandle, INIT_ORDINAL);
	if (!initProc)
		return false;
	mouseProc = (HOOKPROC)GetProcAddress(dllHandle, MOUSEPROC_ORDINAL);
	if (!mouseProc)
		return false;
	return true;
}

/* Attaches global event hooks.
 * Currently this is only done for mouse events; keyboard events may be added later.
 * Returns true on success.
 */
bool attachHooks() {
	mouseHook = SetWindowsHookEx(WH_MOUSE, (HOOKPROC)mouseProc, dllHandle, NULL);
	if (!mouseHook)
		return false;
	return true;
}

/* Detaches previously set hooks.
 * Returns true on success.
 */
bool detachHooks() {
	bool success = true;
	if (!UnhookWindowsHookEx(mouseHook))
		success = false;
	return success;
}

/* Shows a messagebox with the message corresponding to GetLastError(), with the given title.
 */
void showLastError(LPCWSTR title) {
	PVOID msg;
	FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, (LPWSTR)&msg, 0, NULL);
	MessageBoxW(NULL, (LPCWSTR)msg, title, MB_OK | MB_ICONERROR);
	LocalFree(msg);
}

/* The main function for the application.
 */
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	int retVal = 0; // value to be returned eventually, after cleaning up etc.
	// First, load the DLL with the event handlers in it.
	if (!loadDll()) {
		showLastError(L"Error loading DLL");
	} else {
		// DLL loaded, get pointers to the functions in it that we need.
		if (!findProcs()) {
			showLastError(L"Error getting handler address");
		} else {
			// Function pointers acquired, initialize the DLL.
			DWORD prevThreadId = initDll();
			if (prevThreadId) {
				// Somebody has been there before us.
				// Kick that instance in the nuts.
				MessageBoxW(NULL, L"Taekwindow is already running and will now be stopped.\n\nRerun the program it if you want to start it again.", L"Taekwindow already running", MB_OK | MB_ICONINFORMATION);
				PostThreadMessage(prevThreadId, WM_QUIT, 0, 0);
			} else {
				// We're the first to initialize the DLL, continue.
				if (!attachHooks()) {
					showLastError(L"Error attaching hook");
				} else {
					MSG msg;
					BOOL getMsgRetVal;

					// main message loop
					do {
						getMsgRetVal = GetMessage(&msg, NULL, 0, 0);
						if (retVal == -1) {
							// error in GetMessage... low-level, panic and abort
							retVal = -1;
							break;
						} else {
							TranslateMessage(&msg);
							DispatchMessage(&msg);
						}
					} while (getMsgRetVal);
					retVal = (int)msg.wParam;

					// normal exit
					detachHooks();
				}
			}
		}
		unloadDll();
	}
	return retVal;
}
