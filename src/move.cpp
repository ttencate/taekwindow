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

	d_parent = GetAncestor(window(), GA_PARENT);

	RECT rect;
	GetWindowRect(window(), &rect);
	d_windowPos.x = rect.left;
	d_windowPos.y = rect.top;

	cursorWindow().setCursor(crMove);
}

void MoveState::exit() {
	DEBUGLOG("Ending move action");
	DeformState::exit();
}

/* Moves the window accordingly.
 */
bool MoveState::onMouseMove(MouseMoveEvent const &event) {
	DeformState::onMouseMove(event);

	POINT delta = mouseDelta();
	d_windowPos.x += delta.x;
	d_windowPos.y += delta.y;
	moveWindow();

	return true;
}

void MoveState::moveWindow() {
	POINT clientPos = d_windowPos;
	if (d_parent) {
		ScreenToClient(d_parent, &clientPos);
	}
	SetWindowPos(window(), 0, clientPos.x, clientPos.y, 0, 0, SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOZORDER);
}
