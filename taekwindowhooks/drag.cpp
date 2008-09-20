#define OEMRESOURCE
#include <windows.h>

#include "drag.hpp"
#include "actions.hpp"
#include "config.hpp"
#include "debuglog.hpp"

extern NormalState normalState;
extern MoveState moveState;
extern MaximizedMoveState maximizedMoveState;
extern ResizeState resizeState;
extern IgnoreState ignoreState;

void BaseState::enter() {
	currentState->exit();
	currentState = this;
}

HWND NormalState::findFirstParent(HWND window, bool (*criterium)(HWND)) {
	HWND ancestor = window;
	while (true) {
		DEBUGLOG("Current ancestor is 0x%X", ancestor);
		if (ancestor && ancestor != INVALID_HANDLE_VALUE) {
			if (criterium(ancestor)) {
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

HWND NormalState::findLastParent(HWND window, bool (*criterium)(HWND)) {
	HWND ancestor = window;
	HWND match = NULL;
	while (ancestor && ancestor != INVALID_HANDLE_VALUE) {
		DEBUGLOG("Current ancestor is 0x%X", ancestor);
		if (criterium(ancestor)) {
			DEBUGLOG("We like your style; storing candidate 0x%X", ancestor);
			match = ancestor;
		}
		ancestor = GetAncestor(ancestor, GA_PARENT);
	}
	return match;
}

bool NormalState::windowHasClass(HWND window, wchar_t const *className) {
	const int BUFFER_SIZE = 256;
	wchar_t buffer[BUFFER_SIZE];
	if (GetClassName(window, buffer, BUFFER_SIZE) < BUFFER_SIZE) {
		return wcscmp(buffer, className) == 0;
	} else {
		// It will not be equal to className if it is that long.
		return false;
	}
}

bool NormalState::isFullscreenWindow(HWND window) {
	return false; // TODO
}

bool NormalState::isMovableWindow(HWND window) {
	if (
		isCaptionWindow(window) &&
		!IsZoomed(window) &&
		!isFullscreenWindow(window)
	) {
		// A normal movable window.
		return true;
	}

	// BEGIN HACK for Google Talk
	if (isGoogleTalk(window)) {
		return true;
	}
	// END HACK

	// No reason why this should be considered movable.
	return false;
}

bool NormalState::isMaximizedMovableWindow(HWND window) {
	return
		isCaptionWindow(window) &&
		IsZoomed(window) &&
		!isFullscreenWindow(window);
}

bool NormalState::isResizableWindow(HWND window) {
	if (
		isThickBorderWindow(window) &&
		!IsZoomed(window) &&
		!isFullscreenWindow(window)
	) {
		return true;
	}

	// BEGIN HACK for Google Talk
	if (isGoogleTalk(window)) {
		return true;
	}
	// END HACK

	return false;
}

bool NormalState::isMaximizedResizableWindow(HWND window) {
	return
		isThickBorderWindow(window) &&
		IsZoomed(window) &&
		!isFullscreenWindow(window);
}

bool NormalState::isThickBorderWindow(HWND window) {
	LONG style = GetWindowLong(window, GWL_STYLE);
	if ((style & WS_THICKFRAME) && ((style & WS_BORDER) || !(style & WS_DLGFRAME)))
		return true; // allow resizing of windows with resizable borders only
	else
		return false;
}

bool NormalState::isCaptionWindow(HWND window) {
	LONG style = GetWindowLong(window, GWL_STYLE);
	return (style & WS_CAPTION) == WS_CAPTION;
}

bool NormalState::isGoogleTalk(HWND window) {
	// The contact list window and the chat view window do not have WS_CAPTION style.
	return
		windowHasClass(window, L"Google Talk - Google Xmpp Client GUI Window") ||
		windowHasClass(window, L"Chat View");
}

bool NormalState::isModifierDown() {
	return GetAsyncKeyState(config.modifier) & 0x8000;
}

bool NormalState::onMouseDown(MouseButton button, HWND window, POINT mousePos) {
	if (!isModifierDown()) {
		// This is not interesting. Discard ASAP.
		return false;
	}
	if (button != config.moveButton && button != config.resizeButton) {
		// Wrong button. Discard.
		return false;
	}
	DEBUGLOG("Handling button down event");
	// Yippee! A Modifier-drag event just started that we want to process (or ignore).
	if (button == config.moveButton) {
		// We prefer windows that are not maximized over those that are, which makes sense in an MDI environment.
		// This would be what the user expected.
		HWND parentWindow = findFirstParent(window, isMovableWindow);
		if (parentWindow) {
			moveState.enter(button, parentWindow, mousePos);
			return true;
		} else {
			// No unmaximized movable window found; look for a maximized one that can be kicked to another monitor.
			// Only top-level windows can be moved to other monitors, I guess.
			// So prefer the outermost (last) parent that is maximized.
			parentWindow = findLastParent(window, isMaximizedMovableWindow);
			if (parentWindow) {
				maximizedMoveState.enter(button, parentWindow, mousePos);
				return true;
			}
		}
	} else if (button == config.resizeButton) {
		// Try to find a parent window that we can resize without unmaximizing.
		// This one is probably the one that the user meant.
		HWND parentWindow = findFirstParent(window, isResizableWindow);
		if (parentWindow) {
			resizeState.enter(button, parentWindow, mousePos);
			return true;
		} else {
			// No unmaximized window; perhaps we have a maximized one?
			// We prefer the outermost, so that a maximized MDI parent is preferred over a maximized MDI child inside it.
			// This makes the most sense, because users often forget that there is an MDI at all when their MDI childs are maximized.
			parentWindow = findLastParent(window, isMaximizedResizableWindow);
			if (parentWindow) {
				resizeState.enter(button, parentWindow, mousePos);
				return true;
			}
		}
	} else {
		// Nothing of interest: unused button.
		return false;
	}
	// Unsuitable window. Ignore.
	ignoreState.enter(button);
	return false;
}

void MouseDownState::enter(MouseButton button) {
	DEBUGLOG("Handling button up event");
	BaseState::enter();
	downButton = button;
}

bool MouseDownState::onMouseUp(MouseButton button, HWND window, POINT mousePos) {
	if (button == downButton) {
		normalState.enter();
	}
	return true;
}

void DeformState::enter(MouseButton button, HWND parentWindow, POINT mousePos) {
	MouseDownState::enter(button);
	// Store window handle and Z order position of the victim.
	draggedWindow = parentWindow;
	prevInZOrder = GetNextWindow(parentWindow, GW_HWNDPREV);
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
}

void DeformState::exit() {
	MouseDownState::exit();
	ReleaseCapture();
	if (lastForegroundWindow && lastForegroundWindow != draggedWindow) {
		// The active window was deactivated when we clicked the dragged window.
		// Restore the previously active window to active.
		activateWithoutRaise(lastForegroundWindow);
	}
}

POINT DeformState::mouseDelta(POINT const &mousePos) {
	// Find out the movement since the last known mouse position.
	POINT delta;
	delta.x = mousePos.x - lastMousePos.x;
	delta.y = mousePos.y - lastMousePos.y;
	// Store the current mouse position as last known.
	lastMousePos = mousePos;
	// Return the relative movement.
	return delta;
}

void DeformState::updateWindowPos(UINT flags) {
	SetWindowPos(draggedWindow, prevInZOrder, lastRect.left, lastRect.top, lastRect.right - lastRect.left, lastRect.bottom - lastRect.top, SWP_NOACTIVATE | flags);
}

void DeformState::setCursor(int ocr) {
	HCURSOR newCursor = (HCURSOR)LoadImage(NULL, MAKEINTRESOURCE(ocr), IMAGE_CURSOR, 0, 0, LR_SHARED);
	prevCursor = SetCursor(newCursor);
}

void DeformState::updateCursor(int ocr) {
	HCURSOR newCursor = (HCURSOR)LoadImage(NULL, MAKEINTRESOURCE(ocr), IMAGE_CURSOR, 0, 0, LR_SHARED);
	DestroyCursor(SetCursor(newCursor));
}

void DeformState::restoreCursor() {
	DestroyCursor(SetCursor(prevCursor));
}

void MoveState::enter(MouseButton button, HWND parentWindow, POINT mousePos) {
	DEBUGLOG("Starting move action");
	DeformState::enter(button, parentWindow, mousePos);
	setCursor(OCR_SIZEALL);
}

void MoveState::exit() {
	DEBUGLOG("Ending move action");
	DeformState::exit();
	restoreCursor();
}

bool MoveState::onMouseMove(POINT mousePos) {
	DEBUGLOG("Handling move action");
	POINT delta = mouseDelta(mousePos);
	lastRect.left += delta.x;
	lastRect.top += delta.y;
	lastRect.right += delta.x;
	lastRect.bottom += delta.y;
	updateWindowPos(SWP_NOSIZE);
	return true;
}

void MaximizedMoveState::enter(MouseButton button, HWND parentWindow, POINT mousePos) {
	DEBUGLOG("Starting maximized move action");
	DeformState::enter(button, parentWindow, mousePos);
	// Remember the monitor that currently contains the window.
	currentMonitor = MonitorFromWindow(parentWindow, MONITOR_DEFAULTTONULL);
	setCursor(OCR_SIZEALL);
}

void MaximizedMoveState::exit() {
	DEBUGLOG("Ending maximized move action");
	DeformState::exit();
	restoreCursor();
}

bool MaximizedMoveState::onMouseMove(POINT mousePos) {
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

void ResizeState::enter(MouseButton button, HWND parentWindow, POINT mousePos) {
	DEBUGLOG("Starting resize action");
	DeformState::enter(button, parentWindow, mousePos);
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
	switch (config.resizeMode) {
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
	setCursor(getResizingCursor());
}

void ResizeState::exit() {
	DEBUGLOG("Ending resize action");
	DeformState::exit();
	restoreCursor();
}

bool ResizeState::onMouseMove(POINT mousePos) {
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
		updateCursor(getResizingCursor());
	updateWindowPos(flags);
	return true;
}

void ResizeState::setResizingX(POINT const &pt) {
	resizingX = pt.x * 3 / (lastRect.right - lastRect.left) - 1;
}

void ResizeState::setResizingY(POINT const &pt) {
	resizingY = pt.y * 3 / (lastRect.bottom - lastRect.top) - 1;
}

int ResizeState::getResizingCursor() {
	if (resizingX && !resizingY)
		return OCR_SIZEWE;
	if (!resizingX && resizingY)
		return OCR_SIZENS;
	if (resizingX * resizingY > 0)
		return OCR_SIZENWSE;
	if (resizingX * resizingY < 0)
		return OCR_SIZENESW;
	return OCR_NORMAL; // fallback
}
