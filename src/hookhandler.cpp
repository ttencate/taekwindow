#include "hookhandler.hpp"


bool MouseHookHandler::llMouseDown(LLMouseDownEvent const &) {
	return false;
}

bool MouseHookHandler::llMouseUp(LLMouseUpEvent const &) {
	return false;
}

bool MouseHookHandler::llMouseMove(LLMouseMoveEvent const &) {
	return false;
}

bool MouseHookHandler::llMouseWheel(LLMouseWheelEvent const &) {
	return false;
}

bool MouseHookHandler::handleMessage(UINT, WPARAM, LPARAM) {
	return false;
}
