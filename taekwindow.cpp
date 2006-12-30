#include <windows.h>
#include <stdio.h>

const LPCSTR INIT_ORDINAL = (LPCSTR)1;
const LPCSTR MOUSEPROC_ORDINAL = (LPCSTR)3;

HMODULE dllHandle = NULL;
FARPROC mouseProc = NULL;
HHOOK mouseHook = NULL;

bool loadDll() {
	dllHandle = LoadLibrary(L"taekwindowhooks.dll");
	if (dllHandle) {
		void (__stdcall *initProc)(DWORD) = (void (__stdcall*)(DWORD))GetProcAddress(dllHandle, INIT_ORDINAL);
		if (initProc) {
			(*initProc)(GetCurrentThreadId());
		} else {
			return false;
		}
		return true;
	} else {
		return false;
	}
}

bool unloadDll() {
	return FreeLibrary(dllHandle) != 0;
}

bool findProcs() {
	mouseProc = GetProcAddress(dllHandle, MOUSEPROC_ORDINAL);
	if (!mouseProc)
		return false;
	return true;
}

bool attachHooks() {
	mouseHook = SetWindowsHookEx(WH_MOUSE, (HOOKPROC)mouseProc, dllHandle, NULL);
	if (!mouseHook)
		return false;
	return true;
}

bool detachHooks() {
	bool success = true;
	if (!UnhookWindowsHookEx(mouseHook))
		success = false;
	return success;
}

void showLastError(LPCWSTR title) {
	PVOID msg;
	FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, (LPWSTR)&msg, 0, NULL);
	MessageBoxW(NULL, (LPCWSTR)msg, title, MB_OK | MB_ICONERROR);
	LocalFree(msg);
}

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	// TODO: use mutex to prevent multiple instances
	int retVal = 0;
	if (!loadDll()) {
		showLastError(L"Error loading DLL");
	} else {
		if (!findProcs()) {
			showLastError(L"Error getting handler address");
		} else {
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
		unloadDll();
	}
	return retVal;
}
