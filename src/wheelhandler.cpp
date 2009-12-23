#include <windowsx.h>

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

	// Unfortunately, we do not receive the complete information that is normally in a
	// WM_MOUSEWHEEL or WM_MOUSEHWHEEL message. In particular, the key-down information is missing.
	// It might be in the "reserved" low word of mouseData, but we cannot be sure.
	// Reconstruct it.
	WPARAM wParam = GET_WHEEL_DELTA_WPARAM(event.mouseData) << 16;
	if (GetAsyncKeyState(VK_CONTROL)  & 0x8000) wParam |= MK_CONTROL;
	if (GetAsyncKeyState(VK_LBUTTON)  & 0x8000) wParam |= MK_LBUTTON;
	if (GetAsyncKeyState(VK_MBUTTON)  & 0x8000) wParam |= MK_MBUTTON;
	if (GetAsyncKeyState(VK_RBUTTON)  & 0x8000) wParam |= MK_RBUTTON;
	if (GetAsyncKeyState(VK_SHIFT)    & 0x8000) wParam |= MK_SHIFT;
	if (GetAsyncKeyState(VK_XBUTTON1) & 0x8000) wParam |= MK_XBUTTON1;
	if (GetAsyncKeyState(VK_XBUTTON2) & 0x8000) wParam |= MK_XBUTTON2;

	DEBUGLOG("Mouse position for wheel event: %d, %d", event.mousePos.x, event.mousePos.y);
	LPARAM lParam = (event.mousePos.y << 16) | (event.mousePos.x & 0xFFFF);
	DEBUGLOG("Our lParam converts back to: %d, %d", GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

	DEBUGLOG("Forwarding mouse wheel to window 0x%X (wParam = 0x%08X, lParam = 0x%08X)", window, wParam, lParam);
	SendMessage(window, event.msg, wParam, lParam);
}
