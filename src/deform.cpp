#include "deform.hpp"
#include "debug.hpp"

DeformState::DeformState(POINT mousePos, MouseButton button, HWND window)
:
	MouseDownState(button),
	d_mousePos(mousePos),
	d_window(window)
{
	d_mouseDelta.x = d_mouseDelta.y = 0;
}

/* Begins the drag action.
 */
void DeformState::enter() {
	MouseDownState::enter();
	DEBUGLOG("Entering DeformState");
}

/* Ends the drag action.
 */
void DeformState::exit() {
	DEBUGLOG("Exited DeformState");
	MouseDownState::exit();
}

bool DeformState::onMouseMove(MouseMoveEvent const &event) {
	d_mouseDelta.x = event.mousePos.x - d_mousePos.x;
	d_mouseDelta.y = event.mousePos.y - d_mousePos.y;
	d_mousePos = event.mousePos;
	return MouseDownState::onMouseMove(event);
}
