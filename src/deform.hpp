#ifndef DEFORM_HPP
#define DEFORM_HPP

#include <windows.h>

#include "dragworker.hpp"

class DeformWorker : public DragWorker {

	protected:

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

		DeformWorker(POINT mousePos, HWND window);

		virtual void move(POINT const &mousePos);

};

#endif
