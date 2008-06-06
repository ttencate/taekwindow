#include "actions.hpp"
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

bool doMouseWheel(HWND window, POINT mousePos, WPARAM wParam) {
	HWND targetWindow = WindowFromPoint(mousePos);
	if (targetWindow != window) {
		DEBUGLOG("Forwarding mouse wheel for window 0x%X to window 0x%X", window, targetWindow);
		LPARAM lParam = ((short)(mousePos.x)) | (((short)(mousePos.y)) << 16);
		SendMessage(targetWindow, WM_MOUSEWHEEL, wParam, lParam);
		return true;
	} else {
		// Either the focus happens to be on the window under the cursor,
		// or this is an event we sent ourselves.
		// Either way, we're good, stay away from it.
		return false;
	}
}