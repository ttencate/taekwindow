#ifndef STATE_HPP
#define STATE_HPP

#include <windows.h>

#include "types.hpp"
#include "statemachine.hpp"
#include "events.hpp"

/* An interface for a state in the DragMachine state machine.
 * All these functions have implementations, but they are guaranteed to do nothing.
 */
class DragState {

	friend class StateMachine<DragState>; // for setMachine()

	StateMachine<DragState> *d_machine;

	public:

		DragState();

		virtual void enter();
		virtual void exit();

		virtual bool onMouseDown(MouseDownEvent const &event);
		virtual bool onMouseUp(MouseUpEvent const &event);
		virtual bool onMouseMove(MouseMoveEvent const &event);

	protected:

		void setNextState(DragState *state) { ASSERT(d_machine); d_machine->setNextState(state); }

	private:

		/* We need this for bootstrapping: a StateMachine requires a State for its construction,
		 * and a State requires a StateMachine.
		 * This is called from StateMachine; do not call it from anywhere else.
		 */
		void setMachine(StateMachine<DragState> *machine) { ASSERT(!d_machine); d_machine = machine; }
};


#endif
