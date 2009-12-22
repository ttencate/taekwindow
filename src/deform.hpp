#ifndef DEFORM_HPP
#define DEFORM_HPP

#include <windows.h>

#include "mousedown.hpp"

class DeformState : public MouseDownState {

	/* The point at which the mouse cursor was last seen.
	 */
	POINT d_mousePos;

	/* The step that the mouse cursor last took.
	 */
	POINT d_mouseDelta;

	/* The window that we're dragging.
	 */
	HWND d_window;

	public:

		DeformState(POINT mousePos, MouseButton button, HWND window);

		virtual void enter();
		virtual void exit();

		virtual bool onMouseMove(MouseMoveEvent const &event);

	protected:

		POINT mousePos() const { return d_mousePos; }
		POINT mouseDelta() const { return d_mouseDelta; }
		HWND window() const { return d_window; }

};

#endif
