#include "pushhandler.hpp"
#include "wininfo.hpp"
#include "main.hpp"
#include "debuglog.hpp"

bool PushHandler::onMouseDown(MouseDownEvent const &event) {
	if (event.button == activeConfig.pushBackButton) {
		UINT hitTestCode = SendMessage(event.window, WM_NCHITTEST, 0, MAKELPARAM(event.mousePos.x, event.mousePos.y));
		if (hitTestCode == HTCAPTION) {
			HWND window = GetAncestor(event.window, GA_ROOT);
			pushBack(window);
			return true;
		}
	}
	return false;
}

void PushHandler::pushBack(HWND window) {
	DEBUGLOG("Pushing window 0x%X to the back", window);
	// Problem when calling SetWindowPos on an inactive window:
	// the active window is still deactivated, because Windows somehow sends it a WM_NCACTIVATE message.
	// Hence the following hack: restore the focus of the previous application after the SetWindowPos() call.
	// But alas, since we're in the middle of a focus change, GetForegroundWindow() and GetGUIThreadInfo
	// both give NULL for this window handle... so we keep track of the last known active window in the global
	// lastForegroundWindow, updated upon every mouse movement. Ugh.

	// TODO test if this works, remove above story if so
	HWND activeWindow = GetForegroundWindow();

	// Push the window to the back.
	SetWindowPos(window, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);

	// Reactivate the previous window.
	if (activeWindow && activeWindow != window) {
		activateWithoutRaise(activeWindow);
	}
}