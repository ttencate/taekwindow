#ifndef _DRAG_HPP_
#define _DRAG_HPP_

#include <windows.h>

#include "util.hpp"
#include "types.hpp"

enum DragState {
	/* The state active while nothing is happening.
	 */
	dsNormal,
	/* The state active while the user is moving a window.
	 */
	dsMove,
	/* The state active while the user is moving a maximized window.
	 * This is only possible across monitors; the window will retain its maximized state.
	 */
	dsMaximizedMove,
	/* The state active while the user is resizing a window.
	 */
	dsResize,
	/* The ignore state, activated when the user tries to move/resize a window that is not movable or resizable.
	 * This state makes sure that those events are not passed to the application.
	 */
	dsIgnore
};

/* Called when a mouse button is pressed.
 * window is the bottommost window in the hierarchy. mousePos is the location of the mouse cursor.
 * If true is returned, the event will not be passed to the application.
 */
bool onMouseDown(MouseButton button, HWND window, POINT mousePos);

/* Called when a mouse button is released.
 * window is the bottommost window in the hierarchy. mousePos is the location of the mouse cursor.
 * If true is returned, the event will not be passed to the application.
 */
bool onMouseUp(MouseButton button, HWND window, POINT mousePos);

/* Called when the mouse is moved. Overriding functions must call the base function.
 * If true is returned, the event will not be passed to the application.
 */
bool onMouseMove(POINT mousePos);

#endif
