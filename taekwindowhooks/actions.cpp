#include "actions.hpp"
#include "debuglog.hpp"

void doPushBack(HWND window) {
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
		DEBUGLOG("Activating previously active window 0x%X", lastForegroundWindow);
		// Save the Z position of the previously active window.
		HWND insertAfter = GetNextWindow(lastForegroundWindow, GW_HWNDPREV);
		SetForegroundWindow(lastForegroundWindow);
		// This has pulled it to the front; so change its position in the Z order back.
		SetWindowPos(lastForegroundWindow, insertAfter, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
	}
}
