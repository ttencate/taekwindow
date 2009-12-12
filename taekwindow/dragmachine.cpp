#include "dragmachine.hpp"

DragMachine::DragMachine(DragState *startState)
:
	StateMachine<DragState>(startState)
{
}

bool DragMachine::onMouseDown(MouseDownEvent const &event) {
	bool retVal = currentState().onMouseDown(event);
	enterNextState();
	return retVal;
}

bool DragMachine::onMouseUp(MouseUpEvent const &event) {
	bool retVal = currentState().onMouseUp(event);
	enterNextState();
	return retVal;
}

bool DragMachine::onMouseMove(MouseMoveEvent const &event) {
	bool retVal = currentState().onMouseMove(event);
	enterNextState();
	return retVal;
}
