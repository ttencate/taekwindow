#include "move.hpp"
#include "debug.hpp"

MoveState::MoveState(POINT mousePos, MouseButton button, HWND window)
:
	DeformState(mousePos, button, window)
{
}

/* Sets up the cursor.
 */
void MoveState::enter() {
	DeformState::enter();
	DEBUGLOG("Starting move action");

	RECT rect;
	GetWindowRect(window(), &rect);
	d_windowPos.x = rect.left;
	d_windowPos.y = rect.top;

	cursorWindow().setCursor(crMove);
}

/* Moves the window accordingly.
 */
bool MoveState::onMouseMove(MouseMoveEvent const &event) {
	DeformState::onMouseMove(event);
	DEBUGLOG("Handling move action");

	POINT delta = mouseDelta();
	d_windowPos.x += delta.x;
	d_windowPos.y += delta.y;
	moveWindow();

	return true;
}

void MoveState::moveWindow() {
	SetWindowPos(window(), 0, d_windowPos.x, d_windowPos.y, 0, 0, SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOZORDER);
}
