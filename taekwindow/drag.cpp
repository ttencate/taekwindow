#include <windows.h>
#include <tchar.h>

#include "drag.hpp"
#include "wininfo.hpp"
#include "actions.hpp"
#include "main.hpp"
#include "debuglog.hpp"
#include "cursorwindow.hpp"

/* The current state of the state machine.
 * Must only be changed through changeState().
 */
DragState currentState = dsNormal;

/* The button that was pressed down and caused us to be in this state.
 */
MouseButton downButton;

/* The point at which the mouse cursor was last seen.
 */
POINT lastMousePos;

/* The window that we're dragging.
 */
HWND draggedWindow;

/* The cursor window. Only valid while dragging.
 */
CursorWindow *cursorWindow = NULL;

/* The window in the Z-order previous to the draggedWindow.
 * Used to keep the order intact when calling SetWindowPos.
 */
HWND prevInZOrder;

/* The current position of the window. Saves calls to GetWindowRect.
 */
RECT lastRect;

/* The monitor that the window is currently on.
 */
HMONITOR currentMonitor;

/* The side(s) on which the window is resized. Both either -1, 0 or 1.
 */
int resizingX, resizingY;

/* Calls SetWindowPos with the appropriate arguments. Extra flags can be passed in.
 */
void updateWindowPos(UINT flags) {
	SetWindowPos(draggedWindow, prevInZOrder, lastRect.left, lastRect.top, lastRect.right - lastRect.left, lastRect.bottom - lastRect.top, SWP_NOACTIVATE | flags);
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

/* Returns the cursor to be used for the current resizing direction.
 */
Cursor getResizingCursor() {
	if (resizingX && !resizingY)
		return crResizeWE;
	if (!resizingX && resizingY)
		return crResizeNS;
	if (resizingX * resizingY > 0)
		return crResizeNWSE;
	if (resizingX * resizingY < 0)
		return crResizeNESW;
	return crNormal; // fallback
}

/* Returns the movement of the mouse since the last time.
 */
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

/* Returns true if the modifier key (e.g. Alt) is currently pressed.
 */
bool isModifierDown() {
	return (GetAsyncKeyState(activeConfig.modifier) & 0x8000) != 0;
}

// STATE EXITING --------------------------------------------------------------

/* Ends the drag action.
 * Restores the cursor.
 */
void exitDeformState() {
	DEBUGLOG("Ending drag action");
	delete cursorWindow;
	cursorWindow = NULL;
	if (lastForegroundWindow && lastForegroundWindow != draggedWindow) {
		// The active window was deactivated when we clicked the dragged window.
		// Restore the previously active window to active.
		activateWithoutRaise(lastForegroundWindow);
	}
}

/* Called right before the current state is exited.
 */
void exitState() {
	switch (currentState) {
		case dsMove:
		case dsMaximizedMove:
		case dsResize:
			exitDeformState();
			break;
	}
}

// STATE ENTERING -------------------------------------------------------------

/* Switches to the given new state, calling the exit handler for the current state.
 * Is called from the enter*State functions.
 */
void changeState(DragState newState) {
	exitState();
	currentState = newState;
}

/* Stores the button for later use.
 */
void enterMouseDownState(MouseButton button) {
	DEBUGLOG("Handling button down event");
	downButton = button;
}

/* Stores the initial state for later use.
 * Note that parentWindow refers to the window we should move/resize,
 * not the one bottommost window that receives the actual events.
 */
void enterDeformState(MouseButton button, HWND parentWindow, POINT mousePos) {
	enterMouseDownState(button);
	// Store window handle and Z order position of the victim.
	draggedWindow = parentWindow;
	prevInZOrder = GetNextWindow(parentWindow, GW_HWNDPREV);

	// Create the cursor window.
	cursorWindow = new CursorWindow();
	// Store current mouse position.
	lastMousePos = mousePos;
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
}

/* Sets up the cursor.
 */
void enterMoveState(MouseButton button, HWND parentWindow, POINT mousePos) {
	DEBUGLOG("Starting move action");
	changeState(dsMove);
	enterDeformState(button, parentWindow, mousePos);
	cursorWindow->setCursor(crMove);
}

/* Switches back to the normal state.
 */
void enterNormalState() {
	changeState(dsNormal);
}

/* Sets up the cursor.
 */
void enterMaximizedMoveState(MouseButton button, HWND parentWindow, POINT mousePos) {
	DEBUGLOG("Starting maximized move action");
	changeState(dsMaximizedMove);
	enterDeformState(button, parentWindow, mousePos);
	// Remember the monitor that currently contains the window.
	currentMonitor = MonitorFromWindow(parentWindow, MONITOR_DEFAULTTONULL);
	cursorWindow->setCursor(crMove);
}

/* Sets up the cursor and the resize type.
 */
void enterResizeState(MouseButton button, HWND parentWindow, POINT mousePos) {
	DEBUGLOG("Starting resize action");
	changeState(dsResize);
	enterDeformState(button, parentWindow, mousePos);
	if (IsZoomed(parentWindow)) {
		// When resizing a maximized window, unmaximize it first.
		// Set its restored size to its maximized size, but pull the borders onto the screen.
		// When a window is maximized, it is actually slightly off the screen on all sides, to hide its borders.
		GetWindowRect(draggedWindow, &lastRect);
		WINDOWINFO windowInfo;
		windowInfo.cbSize = sizeof(windowInfo);
		GetWindowInfo(draggedWindow, &windowInfo);
		lastRect.left += windowInfo.cxWindowBorders;
		lastRect.top += windowInfo.cyWindowBorders;
		lastRect.right -= windowInfo.cxWindowBorders;
		lastRect.bottom -= windowInfo.cyWindowBorders;
		// Use SetWindowPlacement for demaximizing to prevent animation.
		// And while we're at it, set the size too.
		WINDOWPLACEMENT windowPlacement;
		windowPlacement.length = sizeof(windowPlacement);
		GetWindowPlacement(draggedWindow, &windowPlacement);
		windowPlacement.showCmd = SW_RESTORE;
		windowPlacement.rcNormalPosition = lastRect;
		SetWindowPlacement(draggedWindow, &windowPlacement);
	}
	// Find out at which corner to resize.
	ScreenToClient(draggedWindow, &mousePos);
	switch (activeConfig.resizeMode) {
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
	cursorWindow->setCursor(getResizingCursor());
}

void enterIgnoreState(MouseButton button) {
	DEBUGLOG("Starting ignore action");
	changeState(dsIgnore);
	enterMouseDownState(button);
	cursorWindow->setCursor(crIgnore);
}

// EVENT HANDLING -------------------------------------------------------------

bool onMouseDownNormalState(MouseButton button, HWND window, POINT mousePos) {
	if (!isModifierDown()) {
		// This is not interesting. Discard ASAP.
		return false;
	}
	if (button != activeConfig.moveButton && button != activeConfig.resizeButton) {
		// Wrong button. Discard.
		return false;
	}
	DEBUGLOG("Handling button down event");
	// Yippee! A Modifier-drag event just started that we want to process (or ignore).
	if (button == activeConfig.moveButton) {
		// We prefer windows that are not maximized over those that are, which makes sense in an MDI environment.
		// This would be what the user expected.
		HWND parentWindow = findFirstParent(window, isRestoredMovableWindow);
		if (parentWindow) {
			enterMoveState(button, parentWindow, mousePos);
			return true;
		} else {
			// No unmaximized movable window found; look for a maximized one that can be kicked to another monitor.
			// Only top-level windows can be moved to other monitors, I guess.
			// So prefer the outermost (last) parent that is maximized.
			parentWindow = findLastParent(window, isMaximizedMovableWindow);
			if (parentWindow) {
				enterMaximizedMoveState(button, parentWindow, mousePos);
				return true;
			}
		}
	} else if (button == activeConfig.resizeButton) {
		// Try to find a parent window that we can resize without unmaximizing.
		// This one is probably the one that the user meant.
		HWND parentWindow = findFirstParent(window, isRestoredResizableWindow);
		if (parentWindow) {
			enterResizeState(button, parentWindow, mousePos);
			return true;
		} else {
			// No unmaximized window; perhaps we have a maximized one?
			// We prefer the outermost, so that a maximized MDI parent is preferred over a maximized MDI child inside it.
			// This makes the most sense, because users often forget that there is an MDI at all when their MDI childs are maximized.
			parentWindow = findLastParent(window, isMaximizedResizableWindow);
			if (parentWindow) {
				enterResizeState(button, parentWindow, mousePos);
				return true;
			}
		}
	} else {
		// Nothing of interest: unused button.
		return false;
	}
	// Unsuitable window. Ignore.
	enterIgnoreState(button);
	return false;
}

/* Moves the window accordingly.
 */
bool onMouseMoveMoveState(POINT mousePos) {
	DEBUGLOG("Handling move action");
	POINT delta = mouseDelta(mousePos);
	lastRect.left += delta.x;
	lastRect.top += delta.y;
	lastRect.right += delta.x;
	lastRect.bottom += delta.y;
	updateWindowPos(SWP_NOSIZE);
	return true;
}

/* Moves the window accordingly.
 */
bool onMouseMoveMaximizedMoveState(POINT mousePos) {
	DEBUGLOG("Handling maximized move action");
	HMONITOR mouseMonitor = MonitorFromPoint(mousePos, MONITOR_DEFAULTTONEAREST);
	if (mouseMonitor != currentMonitor) {
		// Window needs to be moved to another monitor, while retaining its maximized state.
		currentMonitor = mouseMonitor;
		// Figure out where the monitor is on the virtual screen.
		MONITORINFO monitorInfo;
		monitorInfo.cbSize = sizeof(monitorInfo);
		GetMonitorInfo(currentMonitor, &monitorInfo);
		DEBUGLOG("Monitor work area at %d,%d (%dx%d)", monitorInfo.rcWork.left, monitorInfo.rcWork.top, monitorInfo.rcWork.right - monitorInfo.rcWork.left, monitorInfo.rcWork.bottom - monitorInfo.rcWork.top);
		// Now move the window by unmaximizing, moving, remaximizing.
		// First, lock drawing to prevent annoying flicker.
		LockWindowUpdate(draggedWindow);
		// Use SetWindowPlacement to change the style to SW_RESTORE, because ShowWindow does animations
		// (simpler ones than AnimateWindow, but still annoying).
		WINDOWPLACEMENT windowPlacement;
		windowPlacement.length = sizeof(windowPlacement);
		GetWindowPlacement(draggedWindow, &windowPlacement);
		windowPlacement.showCmd = SW_RESTORE;
		SetWindowPlacement(draggedWindow, &windowPlacement);
		// Move the restored window to the top left of the working area of the desired monitor.
		SetWindowPos(draggedWindow, prevInZOrder, monitorInfo.rcWork.left, monitorInfo.rcWork.top, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE);
		// Unlock window drawing for the final stage.
		LockWindowUpdate(NULL);
		// And remaximize.
		GetWindowPlacement(draggedWindow, &windowPlacement);
		windowPlacement.showCmd = SW_MAXIMIZE;
		SetWindowPlacement(draggedWindow, &windowPlacement);
	}
	return true;
}

/* Resizes the window accordingly.
 */
bool onMouseMoveResizeState(POINT mousePos) {
	DEBUGLOG("Handling resize action");
	POINT delta = mouseDelta(mousePos);
	// Do not move the window, unless resizing at its top and/or its left.
	UINT flags = SWP_NOMOVE;
	// Resize at the right corner/edge.
	ScreenToClient(draggedWindow, &mousePos);
	bool needCursorUpdate = true;
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
			if (resizingX)
				needCursorUpdate = true;
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
			if (resizingY)
				needCursorUpdate = true;
			break;
	}
	if (needCursorUpdate)
		cursorWindow->setCursor(getResizingCursor());
	updateWindowPos(flags);
	return true;
}

bool onMouseDown(MouseButton button, HWND window, POINT mousePos) {
	switch (currentState) {
		case dsNormal:
			return onMouseDownNormalState(button, window, mousePos);
		default:
			// If we're in some special state, mouse-downs are always ignored.
			return true;
	}
}

bool onMouseUp(MouseButton button, HWND, POINT) {
	switch (currentState) {
		case dsNormal:
			return false;
		default:
			// We're in a state in which a special button was down.
			if (button == downButton) {
				// Go back to normal, but eat the event.
				enterNormalState();
			}
			// Some other button was released; eat that event too.
			return true;
	}
}

bool onMouseMove(POINT mousePos) {
	switch (currentState) {
		case dsNormal:
			return false;
		case dsMove:
			return onMouseMoveMoveState(mousePos);
		case dsMaximizedMove:
			return onMouseMoveMaximizedMoveState(mousePos);
		case dsResize:
			return onMouseMoveResizeState(mousePos);
		case dsIgnore:
			return true;
	}
	return true;
}
