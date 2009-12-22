#include "wheelhandler.hpp"
#include "globals.hpp"
#include "hacks.hpp"
#include "debug.hpp"

bool WheelHandler::onMouseWheel(MouseWheelEvent const &event) {
	if (globals->config().scrollWindowUnderCursor) {
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

	DEBUGLOG("Forwarding mouse wheel to window 0x%X", window);
	
	// Unfortunately, we do not receive the complete information that is normally in a
	// WM_MOUSEWHEEL message. In particular, the key-down information is missing.
	// It might be in the "reserved" low word of mouseData, but we cannot be sure.
	// Reconstruct it.
	WPARAM wParam = GET_WHEEL_DELTA_WPARAM(event.mouseData) << 16;
	if (GetAsyncKeyState(VK_CONTROL))  wParam |= MK_CONTROL;
	if (GetAsyncKeyState(VK_LBUTTON))  wParam |= MK_LBUTTON;
	if (GetAsyncKeyState(VK_MBUTTON))  wParam |= MK_MBUTTON;
	if (GetAsyncKeyState(VK_RBUTTON))  wParam |= MK_RBUTTON;
	if (GetAsyncKeyState(VK_SHIFT))    wParam |= MK_SHIFT;
	if (GetAsyncKeyState(VK_XBUTTON1)) wParam |= MK_XBUTTON1;
	if (GetAsyncKeyState(VK_XBUTTON2)) wParam |= MK_XBUTTON2;

	LPARAM lParam = ((short)(event.mousePos.x)) | (((short)(event.mousePos.y)) << 16);
	SendMessage(window, WM_MOUSEWHEEL, wParam, lParam);
}
