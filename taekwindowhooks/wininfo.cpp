#include <windows.h>
#include <tchar.h>

#include "wininfo.hpp"
#include "debuglog.hpp"

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

	return (lstrcmp(buffer, className) == 0);
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

// BEGIN HACKs for specific applications

bool isGoogleTalk(HWND window) {
	// The contact list window and the chat view window do not have WS_CAPTION style, but is movable/resizable.
	return
		windowHasClass(window, _T("Google Talk - Google Xmpp Client GUI Window")) ||
		windowHasClass(window, _T("Chat View"));
}

bool isGoogleChrome(HWND window) {
	// Google Chrome does not have WS_CAPTION style, but is movable/resizable.
	return windowHasClass(window, _T("Chrome_XPFrame"));
}

bool isMSOfficeDocument(HWND window) {
	// Microsoft Office Word 2007 does internally use MDI, but does not show it,
	// so we pretend that an Office document is not movable/resizable.
	// Microsoft Office Excel 2007 uses an MDI and shows it too, but does its own handling
	// of maximization (i.e. does not set WS_MAXIMIZED).
	// We just pretend that these windows are not floating windows at all,
	// so it is always the parent window that gets manipulated.
	return
		windowHasClass(window, _T("_WwB")) || // Word 2007
		windowHasClass(window, _T("EXCEL7")); // Excel 2007
}

// END HACKs

bool isMovableWindow(HWND window) {
	// BEGIN HACK for MS Office
	if (isMSOfficeDocument(window)) {
		return false;
	}
	// END HACK

	if (isCaptionWindow(window) && !isFullscreenWindow(window)) {
		// A normal movable window.
		return true;
	}

	// BEGIN HACK for Google Talk
	if (isGoogleTalk(window)) {
		return true;
	}
	// END HACK

	// BEGIN HACK for Google Chrome
	if (isGoogleChrome(window)) {
		return true;
	}
	// END HACK

	// No reason why this should be considered movable.
	return false;
}

bool isResizableWindow(HWND window) {
	// BEGIN HACK for MS Office
	if (isMSOfficeDocument(window)) {
		return false;
	}
	// END HACK

	if (isThickBorderWindow(window) && !isFullscreenWindow(window)) {
		return true;
	}

	// BEGIN HACK for Google Talk
	if (isGoogleTalk(window)) {
		return true;
	}
	// END HACK

	// BEGIN HACK for Google Chrome
	if (isGoogleChrome(window)) {
		return true;
	}
	// END HACK

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