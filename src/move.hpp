#ifndef MOVE_HPP
#define MOVE_HPP

#include "deform.hpp"

class MoveState : public DeformState {

	/* May be NULL.
	 */
	HWND d_parent;

	/* Screen coordinates.
	 */
	POINT d_windowPos;

	public:

		MoveState(POINT mousePos, MouseButton button, HWND window);

		virtual void enter();
		virtual void exit();

		virtual bool onMouseMove(MouseMoveEvent const &event);

	private:

		void moveWindow();

};

#endif