#include "pushhandler.hpp"
#include "wininfo.hpp"
#include "globals.hpp"
#include "debug.hpp"

bool PushHandler::onMouseDown(MouseDownEvent const &event) {
	if (event.button == globals->config().pushBackButton) {
		UINT hitTestCode = SendMessage(event.window, WM_NCHITTEST, 0, MAKELPARAM(event.mousePos.x, event.mousePos.y));
		if (hitTestCode == HTCAPTION) {
			pushBack(event.window);
			return true;
		}
	}
	return false;
}

void PushHandler::pushBack(HWND window) {
	DEBUGLOG("Pushing window 0x%X to the back", window);

	HWND activeWindow = GetForegroundWindow();

	// Push the window to the back.
	SetWindowPos(window, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);

	// Reactivate the previous window.
	if (activeWindow && activeWindow != window) {
		activateWithoutRaise(activeWindow);
	}
}