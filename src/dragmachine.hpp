#ifndef DRAGMACHINE_HPP
#define DRAGMACHINE_HPP

#include "statemachine.hpp"
#include "dragstate.hpp"
#include "events.hpp"

class DragMachine : public StateMachine<DragState> {

	public:

		explicit DragMachine(DragState *startState);

		bool onMouseDown(MouseDownEvent const &event);
		bool onMouseUp(MouseUpEvent const &event);
		bool onMouseMove(MouseMoveEvent const &event);

};

#endif
