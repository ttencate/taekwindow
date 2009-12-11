#include <tchar.h>

#include "actions.hpp"
#include "wininfo.hpp"
#include "util.hpp"
#include "debuglog.hpp"

bool doPushBack(HWND window) {
	DEBUGLOG("Pushing window 0x%X to the back", window);
	// Problem when calling SetWindowPos on an inactive window:
	// the active window is still deactivated, because Windows somehow sends it a WM_NCACTIVATE message.
	// Hence the following hack: restore the focus of the previous application after the SetWindowPos() call.
	// But alas, since we're in the middle of a focus change, GetForegroundWindow() and GetGUIThreadInfo
	// both give NULL for this window handle... so we keep track of the last known active window in the global
	// lastForegroundWindow, updated upon every mouse movement. Ugh.

	// Push the window to the back.
	SetWindowPos(window, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);

	// Reactivate the previous window.
	if (lastForegroundWindow && lastForegroundWindow != window) {
		activateWithoutRaise(lastForegroundWindow);
	}

	// Eat the event.
	return true;
}

// BEGIN HACK for mIRC.
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

// END HACK

bool doMouseWheel(HWND window, POINT mousePos, WPARAM wParam) {
	// BEGIN HACK for mIRC
	if (isMIRCWindow(window)) {
		window = findMIRCTextWindow(window);
	}
	// END HACK

	DEBUGLOG("Forwarding mouse wheel to window 0x%X", window);
	LPARAM lParam = ((short)(mousePos.x)) | (((short)(mousePos.y)) << 16);
	SendMessage(window, WM_MOUSEWHEEL, wParam, lParam);
	return true;
}