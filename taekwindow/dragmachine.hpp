#ifndef DRAGMACHINE_HPP
#define DRAGMACHINE_HPP

#include "statemachine.hpp"
#include "dragstate.hpp"

class DragMachine : public StateMachine<DragState> {

	static DragMachine *s_instance;

	public:

		static void init(DragState *startState);
		static void cleanup();
		static DragMachine &instance() { return *s_instance; }

		bool onMouseDown(MouseButton button, HWND window, POINT mousePos);
		bool onMouseUp(MouseButton button, HWND window, POINT mousePos);
		bool onMouseMove(POINT mousePos);

	private:

		DragMachine(DragState *startState);

};

#endif
