#include <tchar.h>

#include "mousehook.hpp"
#include "globals.hpp"
#include "debug.hpp"
#include "winutils.hpp"

MouseHook::MouseHook()
:
	Hook(WH_MOUSE_LL, &llMouseProc)
{
}

/* Clips the given point to be inside the cursor clip rectangle.
 */
POINT MouseHook::clipCursor(POINT const &pos) {
	POINT newPos = pos;
	RECT clip;
	GetClipCursor(&clip);
	if (newPos.x < clip.left)
		newPos.x = clip.left;
	if (newPos.x >= clip.right)
		newPos.x = clip.right;
	if (newPos.y < clip.top)
		newPos.y = clip.top;
	if (newPos.y >= clip.bottom)
		newPos.y = clip.bottom;
	return newPos;
}

bool MouseHook::sendMessageToHandler(UINT message, WPARAM mouseData, POINT const &mousePos) {
	switch (message) {
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
			DEBUGLOG("Hook sending mouse-down message");
			return globals->mouseHandlerList().llMouseDown(LLMouseDownEvent(mousePos, messageToButton(message)));
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
			DEBUGLOG("Hook sending mouse-up message");
			return globals->mouseHandlerList().llMouseUp(LLMouseUpEvent(mousePos, messageToButton(message)));
		case WM_MOUSEMOVE:
			return globals->mouseHandlerList().llMouseMove(LLMouseMoveEvent(mousePos));
		case WM_MOUSEWHEEL:
		case WM_MOUSEHWHEEL:
			return globals->mouseHandlerList().llMouseWheel(LLMouseWheelEvent(message, mousePos, GET_WHEEL_DELTA_WPARAM(mouseData)));
		default:
			return false;
	}
}

bool MouseHook::processMouseMessage(UINT message, WPARAM mouseData, POINT const &mousePos) {
	// A low-level mouse proc gets the mouse coordinates even before they are
	// clipped to the screen boundaries. So we need to do this ourselves.
	POINT pos = clipCursor(mousePos);

	bool eat = sendMessageToHandler(message, mouseData, pos);

	if (eat && message == WM_MOUSEMOVE) {
		// The low-level hook is so low-level that if we eat the event,
		// the mouse cursor position won't be updated.
		SetCursorPos(pos.x, pos.y);
	}

	return eat;
}

LRESULT CALLBACK MouseHook::llMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
	bool eat = false; // Set to true if we don't want to pass the event to the application.
	if (nCode == HC_ACTION) { // If nCode < 0, do nothing as per Microsoft's recommendations.
		MSLLHOOKSTRUCT const &eventInfo = *(MSLLHOOKSTRUCT*)lParam;
		eat = processMouseMessage(wParam, eventInfo.mouseData, eventInfo.pt);
	}

	LRESULT res = CallNextHookEx((HHOOK)37, nCode, wParam, lParam); // first argument ignored
	return eat ? 1 : res; // nonzero return value prevents passing the event to the application
}
