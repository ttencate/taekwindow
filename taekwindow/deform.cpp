#include "deform.hpp"
#include "debuglog.hpp"

DeformState::DeformState(POINT mousePos, MouseButton button, HWND window)
:
	MouseDownState(button),
	d_mousePos(mousePos),
	d_window(window)
{
	d_mouseDelta.x = d_mouseDelta.y = 0;
}

/* Stores the initial state for later use.
 * Note that parentWindow refers to the window we should move/resize,
 * not the one bottommost window that receives the actual events.
 */
void DeformState::enter() {
	MouseDownState::enter();
	DEBUGLOG("Entering DeformState");

	// Store window handle and Z order position of the victim.
	d_prevInZOrder = GetNextWindow(d_window, GW_HWNDPREV);

	// Store the current window rectangle, specified in the client coordinates of the window's parent
	// (or, if no parent, in screen coordinates).
	GetWindowRect(d_window, &d_lastRect);
	HWND parent = GetAncestor(d_window, GA_PARENT);
	if (parent) {
		POINT topLeft = { d_lastRect.left, d_lastRect.top };
		POINT bottomRight = { d_lastRect.right, d_lastRect.bottom };
		ScreenToClient(parent, &topLeft);
		ScreenToClient(parent, &bottomRight);
		d_lastRect.left = topLeft.x;
		d_lastRect.top = topLeft.y;
		d_lastRect.right = bottomRight.x;
		d_lastRect.bottom = bottomRight.y;
	}
}

/* Ends the drag action.
 * Restores the Z order.
 */
void DeformState::exit() {
	DEBUGLOG("Exiting DeformState");
	// TODO remove this code, if it is now unnecessary
	/*
	if (d_lastForegroundWindow && d_lastForegroundWindow != d_window) {
		// The active window was deactivated when we clicked the dragged window.
		// Restore the previously active window to active.
		activateWithoutRaise(d_lastForegroundWindow);
	}
	*/
	MouseDownState::exit();
}

bool DeformState::onMouseMove(POINT mousePos) {
	DEBUGLOG("Handling DeformState::onMouseMove");
	d_mouseDelta.x = mousePos.x - d_mousePos.x;
	d_mouseDelta.y = mousePos.y - d_mousePos.y;
	d_mousePos = mousePos;
	return MouseDownState::onMouseMove(mousePos);
}

void DeformState::updateWindowPos(RECT rect, UINT flags) {
	d_lastRect = rect;
	SetWindowPos(d_window, d_prevInZOrder, d_lastRect.left, d_lastRect.top, d_lastRect.right - d_lastRect.left, d_lastRect.bottom - d_lastRect.top, SWP_NOACTIVATE | flags);
}