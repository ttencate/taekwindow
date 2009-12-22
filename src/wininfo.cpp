#include <windows.h>
#include <tchar.h>

#include "wininfo.hpp"
#include "debuglog.hpp"
#include "hacks.hpp"

HWND findFirstParent(HWND window, bool (*criterium)(HWND)) {
	HWND ancestor = window;
	while (ancestor) {
		DEBUGLOG("Current ancestor is 0x%X", ancestor);
		if (ancestor && ancestor != INVALID_HANDLE_VALUE) {
			if (criterium(ancestor)) {
				DEBUGLOG("We like your style; returning 0x%X", ancestor);
				break;
			}
		} else {
			// No parent window.
			DEBUGLOG("Window has no parent; returning NULL");
			ancestor = NULL;
			break;
		}
		ancestor = GetAncestor(ancestor, GA_PARENT);
	}
	return ancestor;
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

	return (lstrcmp(buffer, className) == 0);
}

bool isMaximizedWindow(HWND window) {
	return IsZoomed(window) == TRUE;
}

bool isFullscreenWindow(HWND) {
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

bool isMovableWindow(HWND window) {
	if (isMSOfficeDocument(window)) {
		return false;
	}

	if (isCaptionWindow(window) && !isFullscreenWindow(window)) {
		// A normal movable window.
		return true;
	}

	if (isGoogleTalk(window) || isGoogleChrome(window)) {
		return true;
	}

	// No reason why this should be considered movable.
	return false;
}

bool isResizableWindow(HWND window) {
	if (isMSOfficeDocument(window)) {
		return false;
	}

	if (isThickBorderWindow(window) && !isFullscreenWindow(window)) {
		return true;
	}

	if (isGoogleTalk(window) || isGoogleChrome(window)) {
		return true;
	}

	return false;
}

bool isRestoredMovableWindow(HWND window) {
	return isMovableWindow(window) && !isMaximizedWindow(window);
}

bool isRestoredResizableWindow(HWND window) {
	return isResizableWindow(window) && !isMaximizedWindow(window);
}

bool isMaximizedMovableWindow(HWND window) {
	return isMovableWindow(window) && isMaximizedWindow(window);
}

bool isMaximizedResizableWindow(HWND window) {
	return isResizableWindow(window) && isMaximizedWindow(window);
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

bool screenToClient(HWND window, RECT &rect) {
	POINT topLeft = { rect.left, rect.top };
	POINT bottomRight = { rect.right, rect.bottom };
	int r1 = ScreenToClient(window, &topLeft);
	int r2 = ScreenToClient(window, &bottomRight);
	rect.left = topLeft.x;
	rect.top = topLeft.y;
	rect.right = bottomRight.x;
	rect.bottom = bottomRight.y;
	return r1 && r2;
}
