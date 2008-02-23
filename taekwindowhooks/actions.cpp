#include "actions.hpp"
#include "debuglog.hpp"

void doPushBack(HWND window) {
	DEBUGLOG("Pushing window 0x%X to the back", window);
	// Problem when calling SetWindowPos on an inactive window:
	// the active window is still deactivated, because Windows somehow sends it a WM_NCACTIVATE message.
	// Hence the following hack: restore the focus of the previous application after the SetWindowPos() call.
	// But alas, since we're in the middle of a focus change, GetForegroundWindow() and GetGUIThreadInfo
	// both give NULL for this window handle...

	// Push the window to the back.
	SetWindowPos(window, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);

	// Reactivate the previous window.
	if (lastForegroundWindow) {
		DEBUGLOG("Activating previously active window 0x%X", lastForegroundWindow);
		SetForegroundWindow(lastForegroundWindow);
	}
}
