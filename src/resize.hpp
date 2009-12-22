#ifndef RESIZE_HPP
#define RESIZE_HPP

#include "deform.hpp"

class ResizeState : public DeformState {

	/* The current window rectangle, and the desired rectangle,
	 * in client coordinates (or screen coordinates, if no parent).
	 * A possible difference between these is due to minimum/maximum size.
	 */
	RECT d_actualRect, d_desiredRect;

	/* The side(s) on which the window is resized. Both either -1, 0 or 1.
	 */
	int d_resizingX, d_resizingY;

	/* The window's parent, or NULL if none.
	 */
	HWND d_parent;

	public:

		ResizeState(POINT mousePos, MouseButton button, HWND parentWindow);

		virtual void enter();

		virtual bool onMouseMove(MouseMoveEvent const &event);

	private:

		/* Updates d_resizingX and d_resizingY, and returns true if they changed.
		 */
		bool updateResizingXY();

		/* Helpers for updateResizingXY().
		 */
		void updateResizingBottomRight();
		void updateResizingNineRects();

		/* Returns the cursor to be used for the current resizing direction.
		 */
		Cursor getResizingCursor();

		/* Unmaximizes the window. Assumes it's maximized.
		 * This does not depend on d_actualRect or d_desiredRect, and does not update them.
		 */
		void restore();

		/* Resizes the window to the best of its abilities (respecting minimum/maximum size)
		 * to the size of d_desiredRect. Does not update d_actualRect.
		 */
		void resizeWindow();

		/* Returns the current window rectangle, in coordinates suitable for d_actualRect.
		 */
		RECT currentRect() const;

};

#endif
