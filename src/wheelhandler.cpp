#include <windowsx.h>

#include "wheelhandler.hpp"
#include "globals.hpp"
#include "hacks.hpp"
#include "debug.hpp"
#include "winutils.hpp"

bool WheelHandler::llMouseWheel(LLMouseWheelEvent const &event) {
	if (globals->config().scrollWindowUnderCursor) {
		globals->workerThread().postMessage(event.msg, event.delta, pointToDword(event.mousePos));
		return true;
	} else {
		return false;
	}
}

bool WheelHandler::handleMessage(UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_MOUSEWHEEL:
		case WM_MOUSEHWHEEL:
			forwardMouseWheel(message, wParam, lParam);
			return true;
	}
	return false;
}

void WheelHandler::forwardMouseWheel(UINT message, WPARAM wParam, LPARAM lParam) {
	POINT mousePos = dwordToPoint(lParam);
	HWND window = WindowFromPoint(mousePos);
	// BEGIN HACK for mIRC
	if (isMIRCWindow(window)) {
		window = findMIRCTextWindow(window);
	}
	// END HACK

	// Unfortunately, we do not receive the complete information that is normally in a
	// WM_MOUSEWHEEL or WM_MOUSEHWHEEL message. In particular, the key-down information is missing.
	// Reconstruct it.
	wParam = MAKEWPARAM(currentVirtualKeys(), wParam);

	DEBUGLOG("Forwarding mouse wheel to window 0x%X (wParam = 0x%08X, lParam = 0x%08X)", window, wParam, lParam);
	SendMessage(window, message, wParam, lParam);
}
