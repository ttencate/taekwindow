#ifndef STATEMACHINE_HPP
#define STATEMACHINE_HPP

#include <typeinfo>

#include "debuglog.hpp"

/* The StateMachine owns every state passed in, and will take care of deleting them.
 * The State type must have member functions enter() and exit().
 */
template<typename State>
class StateMachine {

	State *d_currentState;
	State *d_nextState;

	public:

		StateMachine(State *start);
		~StateMachine();

		void setNextState(State *nextState);

	protected:

		State &currentState() const { return *d_currentState; }
		void enterNextState();

	private:

		StateMachine(StateMachine const &); // not implemented
		StateMachine &operator=(StateMachine const &); // not implemented
};

template<typename State>
StateMachine<State>::StateMachine(State *startState)
:
	d_currentState(startState),
	d_nextState(0)
{
	DEBUGLOG("Entering start state 0x%08x", d_currentState);
	DEBUGLOG("Address of d_nextState: 0x%08x", &d_nextState);
	d_currentState->enter();
}

template<typename State>
StateMachine<State>::~StateMachine() {
	d_currentState->exit();
	delete d_currentState;
	delete d_nextState;
}

template<typename State>
void StateMachine<State>::setNextState(State *nextState) {
	if (d_nextState)
		delete d_nextState;
	d_nextState = nextState;
	DEBUGLOG("Next state set to 0x%08x", d_nextState);
}

template<typename State>
void StateMachine<State>::enterNextState() {
	if (d_nextState) {
		d_currentState->exit();
		delete d_currentState;

		DEBUGLOG("Switching to next state 0x%08x", d_nextState);
		d_currentState = d_nextState;
		d_currentState->enter();

		d_nextState = 0;
	}
}

#endif
