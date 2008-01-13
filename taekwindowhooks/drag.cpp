#include "drag.hpp"
#include "config.hpp"
#include "state.hpp"
#include "debuglog.hpp"

bool isFullscreenWindow(HWND window) {
	return false; // TODO
}

bool isMovableWindow(HWND window) {
	if (IsZoomed(window))
		return false; // disallow moving maximized windows
	if (isFullscreenWindow(window))
		return false; // disallow moving fullscreen windows
	return true;
}

bool isResizableWindow(HWND window) {
	if (IsZoomed(window))
		return false; // do not allow resizing of maximized windows
	if (isFullscreenWindow(window))
		return false; // disallow resizing fullscreen windows
	LONG style = GetWindowLong(window, GWL_STYLE);
	if ((style & WS_THICKFRAME) && ((style & WS_BORDER) || !(style & WS_DLGFRAME)))
		return true; // allow resizing of windows with resizable borders only
	else
		return false;
}

/* Sets the variables resizingX and resizingY to the proper values,
 * considering the screen-coordinate pointer location.
 */
void setResizingX(POINT const &pt) {
	resizingX = (pt.x - lastRect.left) * 3 / (lastRect.right - lastRect.left) - 1;
}
void setResizingY(POINT const &pt) {
	resizingY = (pt.y - lastRect.top) * 3 / (lastRect.bottom - lastRect.top) - 1;
}

/* Initiates a dragging action, be it moving or resizing.
 * Called by startMoveAction and startResizeAction.
 */
void startDragAction(HWND window, POINT mousePos) {
	// Store window handle of the victim.
	draggedWindow = window;
	// Store current mouse position.
	lastMousePos = mousePos;
	// Capture the mouse so it'll still get events even if the mouse leaves the window
	// (could happen while resizing).
	SetCapture(draggedWindow);
	GetWindowRect(draggedWindow, &lastRect);
}

void startMoveAction(HWND window, POINT mousePos) {
	DEBUGLOG("Starting move action");
	currentState = dsMoving;
	startDragAction(window, mousePos);
}

void startResizeAction(HWND window, POINT mousePos) {
	DEBUGLOG("Starting resize action");
	currentState = dsResizing;
	startDragAction(window, mousePos);
	switch (resizeMode) {
		case rmBottomRight:
			resizingX = 1;
			resizingY = 1;
			break;
		case rmNineRectangles:
			// Figure out in which area we're dragging to resize in the proper direction.
			setResizingX(mousePos);
			setResizingY(mousePos);
			break;
	}
}

POINT mouseDelta(POINT const &mousePos) {
	// Find out the movement since the last known mouse position.
	POINT delta;
	delta.x = mousePos.x - lastMousePos.x;
	delta.y = mousePos.y - lastMousePos.y;
	// Store the current mouse position as last known.
	lastMousePos = mousePos;
	// Return the relative movement.
	return delta;
}

void updateWindowPos(UINT flags) {
	SetWindowPos(draggedWindow, NULL, lastRect.left, lastRect.top, lastRect.right - lastRect.left, lastRect.bottom - lastRect.top, SWP_NOACTIVATE | SWP_NOOWNERZORDER | flags);
}

void doMoveAction(POINT mousePos) {
	DEBUGLOG("Handling move action");
	POINT delta = mouseDelta(mousePos);
	lastRect.left += delta.x;
	lastRect.top += delta.y;
	lastRect.right += delta.x;
	lastRect.bottom += delta.y;
	updateWindowPos(SWP_NOSIZE);
}

void doResizeAction(POINT mousePos) {
	DEBUGLOG("Handling resize action");
	POINT delta = mouseDelta(mousePos);
	// Do not move the window, unless resizing at its top and/or its left.
	UINT flags = SWP_NOMOVE;
	// Resize at the right corner/edge.
	switch (resizingX) {
		case -1:
			lastRect.left += delta.x;
			flags &= ~SWP_NOMOVE;
			break;
		case 1:
			lastRect.right += delta.x;
			break;
		case 0:
			// We may have come close to a vertical border in the meantime.
			setResizingX(mousePos);
			break;
	}
	switch (resizingY) {
		case -1:
			lastRect.top += delta.y;
			flags &= ~SWP_NOMOVE;
			break;
		case 1:
			lastRect.bottom += delta.y;
			break;
		case 0:
			// We may have come close to a horizontal border in the meantime.
			setResizingY(mousePos);
			break;
	}
	updateWindowPos(flags);
}

void endDragAction() {
	ReleaseCapture();
	currentState = dsNone;
}

void endMoveAction() {
	DEBUGLOG("Ending move action");
	endDragAction();
}

void endResizeAction() {
	DEBUGLOG("Ending resize action");
	endDragAction();
}
