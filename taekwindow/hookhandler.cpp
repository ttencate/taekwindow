#include "hookhandler.hpp"

bool MouseHookHandler::onMouseDown(MouseDownEvent const &) {
	return false;
}

bool MouseHookHandler::onMouseUp(MouseUpEvent const &) {
	return false;
}

bool MouseHookHandler::onMouseMove(MouseMoveEvent const &) {
	return false;
}

bool MouseHookHandler::onMouseWheel(MouseWheelEvent const &) {
	return false;
}
