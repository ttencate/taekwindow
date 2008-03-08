#include "util.hpp"
#include "debuglog.hpp"

MouseButton eventToButton(WPARAM wParam) {
	switch (wParam) {
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_NCLBUTTONDOWN:
		case WM_NCLBUTTONUP:
			return mbLeft;
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_NCMBUTTONDOWN:
		case WM_NCMBUTTONUP:
			return mbMiddle;
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_NCRBUTTONDOWN:
		case WM_NCRBUTTONUP:
			return mbRight;
	}
	return mbLeft; // fallback
}

void activateWithoutRaise(HWND window) 
{
	DEBUGLOG("Activating window 0x%X without raising", window);
	// Save the Z position of the previously active window.
	HWND insertAfter = GetNextWindow(window, GW_HWNDPREV);
	SetForegroundWindow(window);
	// This has pulled it to the front; so change its position in the Z order back.
	SetWindowPos(window, insertAfter, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
}

#ifdef _DEBUG
void showLastError(LPCWSTR title) {
	if (!GetLastError())
		return;
	PVOID msg;
	FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, (LPWSTR)&msg, 0, NULL);
	MessageBoxW(NULL, (LPCWSTR)msg, title, MB_OK | MB_ICONERROR);
	LocalFree(msg);
}
#endif
