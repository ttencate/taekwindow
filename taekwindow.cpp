#include <windows.h>
#include <stdio.h>

HMODULE dllHandle = NULL;
FARPROC mouseProc = NULL;
HHOOK mouseHook = NULL;

bool loadDll() {
	dllHandle = LoadLibrary(L"taekwindowhooks.dll");
	return dllHandle != NULL;
}

bool unloadDll() {
	return FreeLibrary(dllHandle) != 0;
}

bool findMouseProc() {
	mouseProc = GetProcAddress(dllHandle, "mouseProc");
	return mouseProc != NULL;
}

bool attachHook() {
	mouseHook = SetWindowsHookEx(WH_MOUSE, (HOOKPROC)mouseProc, dllHandle, NULL);
	return mouseHook != NULL;
}

bool detachHook() {
	return UnhookWindowsHookEx(mouseHook) != 0;
}

void showLastError(LPCWSTR title) {
	PVOID msg;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, (LPWSTR)&msg, 0, NULL);
	MessageBoxW(NULL, (LPCWSTR)msg, title, MB_OK | MB_ICONERROR);
	LocalFree(msg);
}

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	int retVal = 0;
	if (!loadDll()) {
		showLastError(L"Error loading DLL");
	} else {
		if (!findMouseProc()) {
			showLastError(L"Error getting mouse handler address");
		} else {
			if (!attachHook()) {
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
				retVal = msg.wParam;

				// normal exit
				detachHook();
			}
		}
		unloadDll();
	}
	return retVal;
}
