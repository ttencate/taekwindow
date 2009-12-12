#include "move.hpp"
#include "debuglog.hpp"

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
	cursorWindow().setCursor(crMove);
}

/* Moves the window accordingly.
 */
bool MoveState::onMouseMove(POINT mousePos) {
	DeformState::onMouseMove(mousePos);
	DEBUGLOG("Handling move action");

	POINT delta = mouseDelta();
	RECT rect = lastRect();
	rect.left += delta.x;
	rect.top += delta.y;
	rect.right += delta.x;
	rect.bottom += delta.y;
	updateWindowPos(rect, SWP_NOSIZE);

	return true;
}
