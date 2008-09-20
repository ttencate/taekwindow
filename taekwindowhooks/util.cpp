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

HWND findFirstParent(HWND window, bool (*criterium)(HWND)) {
	HWND ancestor = window;
	while (true) {
		DEBUGLOG("Current ancestor is 0x%X", ancestor);
		if (ancestor && ancestor != INVALID_HANDLE_VALUE) {
			if (criterium(ancestor)) {
				DEBUGLOG("We like your style; returning 0x%X", ancestor);
				return ancestor;
			}
		} else {
			// No parent window.
			DEBUGLOG("Window has no parent; returning NULL");
			return NULL;
		}
		ancestor = GetAncestor(ancestor, GA_PARENT);
	}
}

HWND findLastParent(HWND window, bool (*criterium)(HWND)) {
	HWND ancestor = window;
	HWND match = NULL;
	while (ancestor && ancestor != INVALID_HANDLE_VALUE) {
		DEBUGLOG("Current ancestor is 0x%X", ancestor);
		if (criterium(ancestor)) {
			DEBUGLOG("We like your style; storing candidate 0x%X", ancestor);
			match = ancestor;
		}
		ancestor = GetAncestor(ancestor, GA_PARENT);
	}
	return match;
}

bool windowHasClass(HWND window, wchar_t const *className) {
	const int BUFFER_SIZE = 256;

	// A simple cache: if the window handle matches the previously fetched one,
	// we do not need to call GetClassName again.
	static HWND prevWindow = NULL;
	static wchar_t buffer[BUFFER_SIZE];
	if (window != prevWindow) {
		prevWindow = window;
		if (!GetClassName(window, buffer, BUFFER_SIZE))
			return false;
	}

	return (wcscmp(buffer, className) == 0);
}

bool isMaximizedWindow(HWND window) {
	return IsZoomed(window);
}

bool isFullscreenWindow(HWND window) {
	return false; // TODO
}

bool isCaptionWindow(HWND window) {
	LONG style = GetWindowLong(window, GWL_STYLE);
	return (style & WS_CAPTION) == WS_CAPTION;
}

bool isThickBorderWindow(HWND window) {
	LONG style = GetWindowLong(window, GWL_STYLE);
	if ((style & WS_THICKFRAME) && ((style & WS_BORDER) || !(style & WS_DLGFRAME)))
		return true; // allow resizing of windows with resizable borders only
	else
		return false;
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
