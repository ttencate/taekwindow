#ifndef RESIZE_HPP
#define RESIZE_HPP

#include "deform.hpp"

class ResizeState : public DeformState {

	/* The side(s) on which the window is resized. Both either -1, 0 or 1.
	 */
	int d_resizingX, d_resizingY;

	public:

		ResizeState(POINT mousePos, MouseButton button, HWND parentWindow);

		virtual void enter();

		virtual bool onMouseMove(MouseMoveEvent const &event);

	private:

		/* Sets the variables d_resizingX and d_resizingY to the proper values,
		 * considering the client-coordinate pointer location.
		 * Note that, unlike lastRect, these are client coordinates of the dragged window itself,
		 * not those of the dragged window's parent!
		 */
		void setResizingX(POINT const &pt);
		void setResizingY(POINT const &pt);

		/* Returns the cursor to be used for the current resizing direction.
		 */
		Cursor getResizingCursor();

};

#endif
