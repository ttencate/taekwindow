#include "dragstate.hpp"

void DragState::enter() {
}

void DragState::exit() {
}

bool DragState::onMouseDown(MouseButton, HWND, POINT) {
	return false;
}

bool DragState::onMouseUp(MouseButton, HWND, POINT) {
	return false;
}

bool DragState::onMouseMove(POINT) {
	return false;
}
