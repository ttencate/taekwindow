#include <windows.h>

#include "hooks.h"

enum DragState { dsNone, dsMoving, dsResizing, dsIgnoring };

#pragma data_seg(".SHARED")
DWORD mainThreadId = 0;
DragState currentState = dsNone;
POINT lastMousePos = { 0, 0 };
HWND draggedWindow = NULL;
RECT lastRect = { 0, 0, 0, 0 };
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

/*
LRESULT __declspec(dllexport) __stdcall keyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode > 0) { // if nCode < 0, do nothing as per Microsoft's recommendations
#ifdef DEBUG
		// use Q key for clean exit (unloading the DLL, and such)
		if (wParam == 'Q') {
			PostThreadMessage(mainThreadId, WM_QUIT, 0, 0);
			return 1; // nonzero to ignore the keypress message
		}
#endif
		if (wParam == VK_MENU) { // Alt key pressed
			bool previouslyDown = lParam & (1 << 30);
			bool beingReleased = lParam & (1 << 31);
			if (!previouslyDown && !beingReleased) {
				altPressed = true;
			} else if(previouslyDown && beingReleased) {
				altPressed = false;
			}
		}
	}
	return CallNextHookEx((HHOOK)37, nCode, wParam, lParam);
}
*/

bool isDraggableWindow(HWND window) {
	WINDOWINFO info;
	info.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(window, &info);
	if (info.dwStyle & WS_MAXIMIZE) {
		return false;
	} else {
		return true;
	}
}

enum MouseButton { mbLeft, mbMiddle, mbRight };

bool processButtonDown(MouseButton button, MOUSEHOOKSTRUCT *eventInfo) {
	if (currentState == dsNone) {
		bool altDown = GetKeyState(VK_MENU) & 0x8000; // GetAsyncKeyState?
		if (altDown) {
			if (button == mbLeft) {
				currentState = dsMoving;
			} else if (button == mbRight) {
				currentState = dsResizing;
			}
			if (currentState != dsNone) {
				draggedWindow = GetAncestor(eventInfo->hwnd, GA_ROOT);
				if (isDraggableWindow(draggedWindow)) {
					SetCapture(draggedWindow);
					GetWindowRect(draggedWindow, &lastRect);
				} else {
					// when trying to Alt-drag an invalid window, the user won't expect her actions to be passed to that window;
					// so we suppress all events until the mouse button is released
					currentState = dsIgnoring;
				}
				return true;
			}
		}
	}
	return false;
}

bool processButtonUp(MouseButton button) {
	if (currentState != dsNone) {
		ReleaseCapture();
		currentState = dsNone;
		return true;
	}
	return false;
}

LRESULT __declspec(dllexport) __stdcall mouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
	bool processed = false;
	if (nCode >= 0 && nCode == HC_ACTION) { // if nCode < 0, do nothing as per Microsoft's recommendations
		MOUSEHOOKSTRUCT *eventInfo = (MOUSEHOOKSTRUCT*)lParam;
		switch (wParam) {
			case WM_LBUTTONDOWN:
				processed = processButtonDown(mbLeft, eventInfo);
				break;
			case WM_RBUTTONDOWN:
				processed = processButtonDown(mbRight, eventInfo);
				break;
			case WM_LBUTTONUP:
				processed = processButtonUp(mbLeft);
				break;
			case WM_RBUTTONUP:
				processed = processButtonUp(mbRight);
				break;
			case WM_MOUSEMOVE:
				int deltaX, deltaY;
				switch (currentState) {
					case dsMoving:
					case dsResizing:
						deltaX = eventInfo->pt.x - lastMousePos.x, deltaY = eventInfo->pt.y - lastMousePos.y;
						if (currentState == dsMoving) {
							lastRect.left += deltaX;
							lastRect.top += deltaY;
							lastRect.right += deltaX;
							lastRect.bottom += deltaY;
						} else if (currentState == dsResizing) {
							lastRect.right += deltaX;
							lastRect.bottom += deltaY;
						}
						SetWindowPos(draggedWindow, NULL,
							lastRect.left, lastRect.top, lastRect.right - lastRect.left, lastRect.bottom - lastRect.top,
							SWP_NOACTIVATE | SWP_NOOWNERZORDER);
						// no break here
					case dsIgnoring:
						processed = true;
						break;
					case dsNone:
						break;
				}
				break;
		}
		lastMousePos = eventInfo->pt;
	}

	LRESULT res = CallNextHookEx((HHOOK)37, nCode, wParam, lParam); // first argument ignored
	if (processed)
		res = 1;
	return res;
}
