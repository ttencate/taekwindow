#include "draghandler.hpp"
#include "normal.hpp"

DragHandler::DragHandler()
:
	d_machine(new NormalState())
{
}

bool DragHandler::onMouseDown(MouseDownEvent const &event) {
	return d_machine.onMouseDown(event);
}

bool DragHandler::onMouseUp(MouseUpEvent const &event) {
	return d_machine.onMouseUp(event);
}

bool DragHandler::onMouseMove(MouseMoveEvent const &event) {
	return d_machine.onMouseMove(event);
}