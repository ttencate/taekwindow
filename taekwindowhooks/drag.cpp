#include "drag.hpp"
#include "config.hpp"
#include "debuglog.hpp"

/* Whether we're resizing in the x and/or y direction. Only meaningful while dragging.
 * resizingX == -1 means resizing at left border, 0 means not resizing in x direction, 1 means right border.
 * Similar for y; -1 is top border, 0 is no resizing, 1 is bottom border.
 */
extern int resizingX, resizingY;

/* The last known location of the mouse cursor (screen coordinates).
 * This is used in the mouse event handler to compute the distance travelled since the last mouse event.
 */
extern POINT lastMousePos;

/* The window that we are currently dragging.
 * Only meaningful if currentState is dsDragging.
 */
extern HWND draggedWindow;

/* The last known window rectangle of the draggedWindow, in client coordinates.
 * Saves us calls to GetWindowRect() and conversions.
 */
extern RECT lastRect;

HWND findGrabbedParent(HWND window, bool wantResizable) {
	HWND ancestor = window;
	while (true) {
		DEBUGLOG("Current ancestor is 0x%X", ancestor);
		if (ancestor && ancestor != INVALID_HANDLE_VALUE) {
			if (wantResizable && isResizableWindow(ancestor) ||
				!wantResizable && isMovableWindow(ancestor)) {
				DEBUGLOG("We like your style; returning 0x%X", ancestor);
				return ancestor;
			}
		} else {
			// No parent window.
			DEBUGLOG("Window has no parent; returning NULL");
			return NULL;
		}
		ancestor = GetAncestor(ancestor, GA_PARENT);
	}
}

bool isFullscreenWindow(HWND window) {
	return false; // TODO
}

bool isMovableWindow(HWND window) {
	if (IsZoomed(window))
		return false; // disallow moving maximized windows
	if (isFullscreenWindow(window))
		return false; // disallow moving fullscreen windows
	LONG style = GetWindowLong(window, GWL_STYLE);
	if ((style & WS_CAPTION) != WS_CAPTION)
		return false;
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
 * considering the client-coordinate pointer location.
 * Note that, unlike lastRect, these are client coordinates of the dragged window itself,
 * not those of the dragged window's parent!
 */
void setResizingX(POINT const &pt) {
	resizingX = pt.x * 3 / (lastRect.right - lastRect.left) - 1;
}
void setResizingY(POINT const &pt) {
	resizingY = pt.y * 3 / (lastRect.bottom - lastRect.top) - 1;
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
	// Store the current window rectangle, specified in the client coordinates of the window's parent
	// (or, if no parent, in screen coordinates).
	GetWindowRect(draggedWindow, &lastRect);
	HWND parent = GetAncestor(draggedWindow, GA_PARENT);
	if (parent) {
		POINT topLeft = { lastRect.left, lastRect.top }, bottomRight = { lastRect.right, lastRect.bottom };
		ScreenToClient(parent, &topLeft);
		ScreenToClient(parent, &bottomRight);
		lastRect.left = topLeft.x; lastRect.top = topLeft.y;
		lastRect.right = bottomRight.x; lastRect.bottom = bottomRight.y;
	}
	// Notify the window that it's going to be moved/resized.
	// PuTTY, for one, responds to this, by not sending terminal resize events over the network during a resize.
	// Update: don't, because it confuses some apps sometimes.
	// E.g. try to resize the Properties window of Spy++ (which is unresizable),
	// then the main window starts to behave oddly.
	// SendMessage(draggedWindow, WM_ENTERSIZEMOVE, 0, 0);
}

void startMoveAction(HWND window, POINT mousePos) {
	DEBUGLOG("Starting move action");
	startDragAction(window, mousePos);
}

void startResizeAction(HWND window, POINT mousePos) {
	DEBUGLOG("Starting resize action");
	startDragAction(window, mousePos);
	// Find out at which corner to resize.
	ScreenToClient(draggedWindow, &mousePos);
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
	ScreenToClient(draggedWindow, &mousePos);
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
	SendMessage(draggedWindow, WM_SIZING, WMSZ_BOTTOMRIGHT, (LPARAM)&lastRect);
	updateWindowPos(flags);
}

void endDragAction() {
	ReleaseCapture();
	// Notify the window that its moving/resizing is now over.
	// Update: turned off for now, see startDragAction for explanation.
	// SendMessage(draggedWindow, WM_EXITSIZEMOVE, 0, 0);
}

void endMoveAction() {
	DEBUGLOG("Ending move action");
	endDragAction();
}

void endResizeAction() {
	DEBUGLOG("Ending resize action");
	endDragAction();
}
