#ifndef STATE_HPP
#define STATE_HPP

#include <windows.h>

#include "types.hpp"

/* An interface for a state in the DragMachine state machine.
 * All these functions have implementations, but they are guaranteed to do nothing.
 */
class DragState {

	public:

		virtual void enter();
		virtual void exit();

		/* Called when a mouse button is pressed.
		 * window is the bottommost window in the hierarchy. mousePos is the location of the mouse cursor.
		 * If true is returned, the event will not be passed to the application.
		 */
		virtual bool onMouseDown(MouseButton button, HWND window, POINT mousePos);

		/* Called when a mouse button is released.
		 * window is the bottommost window in the hierarchy. mousePos is the location of the mouse cursor.
		 * If true is returned, the event will not be passed to the application.
		 */
		virtual bool onMouseUp(MouseButton button, HWND window, POINT mousePos);

		/* Called when the mouse is moved.
		 * If true is returned, the event will not be passed to the application.
		 */
		virtual bool onMouseMove(POINT mousePos);
};


#endif
