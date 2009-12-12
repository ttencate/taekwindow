#include "dragmachine.hpp"

DragMachine *DragMachine::s_instance = NULL;

void DragMachine::init(DragState *startState) {
	s_instance = new DragMachine(startState);
}

void DragMachine::cleanup() {
	delete s_instance;
	s_instance = NULL;
}

bool DragMachine::onMouseDown(MouseButton button, HWND window, POINT mousePos) {
	bool retVal = currentState().onMouseDown(button, window, mousePos);
	enterNextState();
	return retVal;
}

bool DragMachine::onMouseUp(MouseButton button, HWND window, POINT mousePos) {
	bool retVal = currentState().onMouseUp(button, window, mousePos);
	enterNextState();
	return retVal;
}

bool DragMachine::onMouseMove(POINT mousePos) {
	bool retVal = currentState().onMouseMove(mousePos);
	enterNextState();
	return retVal;
}

DragMachine::DragMachine(DragState *startState)
:
	StateMachine<DragState>(startState)
{
}