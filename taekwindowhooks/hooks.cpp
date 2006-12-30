#include <windows.h>

#include "hooks.h"

#pragma data_seg(".SHARED")
DWORD mainThreadId = 0;
bool altPressed = false;
bool leftButtonPressed = false;
bool rightButtonPressed = false;
POINT lastMousePos;
#pragma data_seg()
#pragma comment(linker, "/section:.SHARED,rws")

HINSTANCE dllHandle = NULL;

void showLastError(LPCWSTR title) {
	if (!GetLastError())
		return;
	PVOID msg;
	FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, (LPWSTR)&msg, 0, NULL);
	MessageBoxW(NULL, (LPCWSTR)msg, title, MB_OK | MB_ICONERROR);
	LocalFree(msg);
}

BOOL WINAPI DllMain(HANDLE hinstDLL, DWORD dwReason, LPVOID lpvReserved) {
	if (dwReason == DLL_PROCESS_ATTACH || dwReason == DLL_THREAD_ATTACH) {
		dllHandle = (HINSTANCE)hinstDLL;
	} else if (dwReason == DLL_PROCESS_DETACH || dwReason == DLL_THREAD_DETACH) {
	}
	return TRUE;
}

void __declspec(dllexport) __stdcall init(DWORD threadId) {
	mainThreadId = threadId;
}

LRESULT __declspec(dllexport) __stdcall keyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode > 0) { // if nCode < 0, do nothing as per Microsoft's recommendations
#ifdef DEBUG
		// use Q key for clean exit (unloading the DLL, and such)
		if (wParam == 'Q') {
			PostThreadMessage(mainThreadId, WM_QUIT, 0, 0);
			return 1; // nonzero to ignore the keypress message
		}
#endif
		if (wParam == VK_MENU /* Alt key */) {
			bool previouslyDown = lParam & (1 << 30);
			bool beingReleased = lParam & (1 << 31);
			if (!previouslyDown && !beingReleased) {
				altPressed = true;
			} else if(previouslyDown && beingReleased) {
				altPressed = false;
			}
		}
	}
	return CallNextHookEx((HHOOK)37 /* ignored */, nCode, wParam, lParam);
}

LRESULT __declspec(dllexport) __stdcall mouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode >= 0 && nCode == HC_ACTION) { // if nCode < 0, do nothing as per Microsoft's recommendations
		switch (wParam) {
			case WM_LBUTTONDOWN:
				leftButtonPressed = true;
				break;
			case WM_LBUTTONUP:
				leftButtonPressed = false;
				break;
			case WM_RBUTTONDOWN:
				rightButtonPressed = true;
				break;
			case WM_RBUTTONUP:
				rightButtonPressed = false;
				break;
		}
		MOUSEHOOKSTRUCT *eventInfo = (MOUSEHOOKSTRUCT*)lParam;
		if (altPressed && (leftButtonPressed || rightButtonPressed)) {
			HWND topLevelWindow = GetAncestor(eventInfo->hwnd, GA_ROOT);
			RECT oldRect;
			GetWindowRect(topLevelWindow, &oldRect);
			int left = oldRect.left, top = oldRect.top, width = oldRect.right - oldRect.left, height = oldRect.bottom - oldRect.top;
			int deltaX = eventInfo->pt.x - lastMousePos.x, deltaY = eventInfo->pt.y - lastMousePos.y;
			if (leftButtonPressed) {
				left += deltaX;
				top += deltaY;
			} else { // rightButtonPressed
				width += deltaX;
				height += deltaY;
			}
			SetWindowPos(topLevelWindow, NULL, left, top, width, height, SWP_NOACTIVATE | SWP_NOOWNERZORDER);
			lastMousePos = eventInfo->pt;
			return 1; // no further processing should be done
		} else {
			lastMousePos = eventInfo->pt;
		}
	}
	// call next hook in chain only if we haven't returned by now
	return CallNextHookEx((HHOOK)37 /* ignored */, nCode, wParam, lParam);
}
