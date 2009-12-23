#include <tchar.h>

#include "hacks.hpp"
#include "wininfo.hpp"

// BEGIN HACKs for specific applications

// The mIRC status and chat windows do not respond to the scroll wheel forwarding
// when they do not have focus. The cause is that WindowFromPoint returns
// the containing MDI child window, instead of the chat text window (of class "Static") inside it.
// When the window doesn't have focus, all is fine. Probably a bug in mIRC.

bool isMIRCWindow(HWND window) {
	return
		windowHasClass(window, _T("mIRC_Status")) ||
		windowHasClass(window, _T("mIRC_Channel")) ||
		windowHasClass(window, _T("mIRC_Query"));
}

HWND findMIRCTextWindow(HWND containerWindow) {
	HWND textWindow = FindWindowEx(containerWindow, NULL, _T("Static"), NULL);
	if (textWindow)
		return textWindow;
	else
		return containerWindow;
}

// The contact list window and the chat view window do not have WS_CAPTION style, but is movable/resizable.
bool isGoogleTalk(HWND window) {
	return
		windowHasClass(window, _T("Google Talk - Google Xmpp Client GUI Window")) ||
		windowHasClass(window, _T("Chat View"));
}

// Google Chrome does not have WS_CAPTION style, but is movable/resizable.
bool isGoogleChrome(HWND window) {
	return windowHasClass(window, _T("Chrome_XPFrame"));
}

// Itunes does not have WS_CAPTION style, but is movable/resizable.
bool isItunes(HWND window) {
	return windowHasClass(window, _T("iTunes"));
}

// Microsoft Office Word 2007 does internally use MDI, but does not show it,
// so we pretend that an Office document is not movable/resizable.
// Microsoft Office Excel 2007 uses an MDI and shows it too, but does its own handling
// of maximization (i.e. does not set WS_MAXIMIZED).
// We just pretend that these windows are not floating windows at all,
// so it is always the parent window that gets manipulated.
bool isMSOfficeDocument(HWND window) {
	return
		windowHasClass(window, _T("_WwB")) || // Word 2007
		windowHasClass(window, _T("EXCEL7")); // Excel 2007
}

// END HACKs
