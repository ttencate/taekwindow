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

	/* The window in the Z-order previous to the draggedWindow.
	 * Used to keep the order intact when calling SetWindowPos.
	 */
	HWND d_prevInZOrder;

	/* The current position of the window. Saves calls to GetWindowRect.
	 */
	RECT d_lastRect;

	public:

		DeformState(POINT mousePos, MouseButton button, HWND window);

		virtual void enter();
		virtual void exit();

		virtual bool onMouseMove(POINT mousePos);

	protected:

		POINT mousePos() const { return d_mousePos; }
		POINT mouseDelta() const { return d_mouseDelta; }

		HWND window() const { return d_window; }
		HWND prevInZOrder() const { return d_prevInZOrder; }
		RECT lastRect() const { return d_lastRect; }

		/* Calls SetWindowPos with the appropriate arguments. Extra flags can be passed in.
		 */
		void updateWindowPos(RECT rect, UINT flags);
};

#endif
