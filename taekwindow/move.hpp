#ifndef MOVE_HPP
#define MOVE_HPP

#include "deform.hpp"

class MoveState : public DeformState {

	public:

		MoveState(POINT mousePos, MouseButton button, HWND window);

		virtual void enter();

		virtual bool onMouseMove(POINT mousePos);

};

#endif