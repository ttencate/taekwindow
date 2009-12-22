#include "dragstate.hpp"

DragState::DragState()
:
	d_machine(NULL)
{
	DEBUGLOG("DragState at 0x%08x: constructor", this);		
}

DragState::~DragState() {
	DEBUGLOG("DragState at 0x%08x: destructor", this);		
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
