#include "dragstate.hpp"

DragState::DragState()
:
	d_machine(NULL)
{
}


void DragState::enter() {
}

void DragState::exit() {
}

bool DragState::onMouseDown(MouseDownEvent const &) {
	return false;
}

bool DragState::onMouseUp(MouseUpEvent const &) {
	return false;
}

bool DragState::onMouseMove(MouseMoveEvent const &) {
	return false;
}
