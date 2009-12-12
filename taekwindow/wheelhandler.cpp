#include "wheelhandler.hpp"
#include "hacks.hpp"
#include "debuglog.hpp"
#include "main.hpp"

bool WheelHandler::onMouseWheel(MouseWheelEvent const &event) {
	if (activeConfig.scrollWindowUnderCursor) {
		forwardMouseWheel(event);
		return true;
	} else {
		// Use Windows default behaviour.
		return false;
	}
}

void WheelHandler::forwardMouseWheel(MouseWheelEvent const &event) {
	HWND window = event.window;
	// BEGIN HACK for mIRC
	if (isMIRCWindow(event.window)) {
		window = findMIRCTextWindow(event.window);
	}
	// END HACK

	// TODO does forwarding of ctrl+scroll work?
	DEBUGLOG("Forwarding mouse wheel to window 0x%X", window);
	WPARAM wParam = event.mouseData;
	LPARAM lParam = ((short)(event.mousePos.x)) | (((short)(event.mousePos.y)) << 16);
	SendMessage(window, WM_MOUSEWHEEL, wParam, lParam);
}
