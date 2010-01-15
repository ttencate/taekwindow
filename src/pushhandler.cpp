#include "pushhandler.hpp"
#include "wininfo.hpp"
#include "globals.hpp"
#include "messages.hpp"
#include "winutils.hpp"
#include "debug.hpp"

bool PushHandler::llMouseDown(LLMouseDownEvent const &event) {
	if (event.button == globals->config().pushBackButton) {
		globals->workerThread().postMessage(PUSH_BACK_MESSAGE, event.button, pointToDword(event.mousePos));
		return true;
	}
	return false;
}

bool PushHandler::handleMessage(UINT message, WPARAM wParam, LPARAM lParam) {
	if (message == PUSH_BACK_MESSAGE) {
		POINT mousePos = dwordToPoint(lParam);
		HWND window = WindowFromPoint(mousePos);
		UINT hitTestCode = SendMessage(window, WM_NCHITTEST, 0, MAKELPARAM(mousePos.x, mousePos.y));
		if (hitTestCode == HTCAPTION) {
			pushBack(window);
			return true;
		} else {
			forwardMouseDown(window, (MouseButton)wParam, mousePos);
			return false;
		}
	} else {
		return false;
	}
}

void PushHandler::pushBack(HWND window) {
	DEBUGLOG("Pushing window 0x%X to the back", window);
	SetWindowPos(window, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
}

void PushHandler::forwardMouseDown(HWND window, MouseButton button, POINT const &mousePos) {
	DEBUGLOG("Push-back not on title bar; forwarding mouse-down event of button %i at (%i, %i)", button, mousePos.x, mousePos.y);
	POINT clientPos = mousePos;
	ScreenToClient(window, &clientPos);
	PostMessage(window, buttonDownMessage(button), currentVirtualKeys(), pointToDword(clientPos));
}
