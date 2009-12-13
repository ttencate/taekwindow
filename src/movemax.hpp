#ifndef MOVEMAX_HPP
#define MOVEMAX_HPP

#include "deform.hpp"

class MoveMaxState : public DeformState {

	/* The monitor that the window is currently on.
	 */
	HMONITOR d_currentMonitor;

	public:

		MoveMaxState(POINT mousePos, MouseButton button, HWND window);

		virtual void enter();

		virtual bool onMouseMove(MouseMoveEvent const &event);

};

#endif
