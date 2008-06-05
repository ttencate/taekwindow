#define OEMRESOURCE
#include <windows.h>

#include "drag.hpp"
#include "actions.hpp"
#include "config.hpp"
#include "debuglog.hpp"

extern NormalState normalState;
extern MoveState moveState;
extern ResizeState resizeState;
extern IgnoreState ignoreState;

void BaseState::enter() {
	currentState->exit();
	currentState = this;
}

HWND NormalState::findGrabbedParent(HWND window, bool wantResizable) {
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

bool NormalState::isFullscreenWindow(HWND window) {
	return false; // TODO
}

bool NormalState::isMovableWindow(HWND window) {
	if (IsZoomed(window))
		return false; // disallow moving maximized windows
	if (isFullscreenWindow(window))
		return false; // disallow moving fullscreen windows
	LONG style = GetWindowLong(window, GWL_STYLE);
	if ((style & WS_CAPTION) != WS_CAPTION)
		return false;
	return true;
}

bool NormalState::isResizableWindow(HWND window) {
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

bool NormalState::isModifierDown() {
	return GetAsyncKeyState(config.modifier) & 0x8000;
}

bool NormalState::onMouseDown(MouseButton button, HWND window, POINT mousePos) {
	if (!isModifierDown()) {
		// This is not interesting. Discard ASAP.
		return false;
	}
	DEBUGLOG("Handling button down event");
	// Yippee! A Modifier-drag event just started that we want to process (or ignore).
	if (button == config.moveButton) {
		// Try to find movable ancestor.
		window = findGrabbedParent(window, false);
		if (window) {
			moveState.enter(button, window, mousePos);
		} else {
			DEBUGLOG("Ignoring button down event because no movable parent was found", window);
			ignoreState.enter(button);
		}
		return true;
	} else if (button == config.resizeButton) {
		// Try to find resizable ancestor.
		window = findGrabbedParent(window, true);
		if (window) {
			resizeState.enter(button, window, mousePos);
		} else {
			DEBUGLOG("Ignoring button down event because no resizable parent was found", window);
			ignoreState.enter(button);
		}
		return true;
	}
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

void DeformState::enter(MouseButton button, HWND window, POINT mousePos) {
	MouseDownState::enter(button);
	// Store window handle and Z order position of the victim.
	draggedWindow = window;
	prevInZOrder = GetNextWindow(window, GW_HWNDPREV);
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

void MoveState::enter(MouseButton button, HWND window, POINT mousePos) {
	DEBUGLOG("Starting move action");
	DeformState::enter(button, window, mousePos);
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

void ResizeState::enter(MouseButton button, HWND window, POINT mousePos) {
	DEBUGLOG("Starting resize action");
	DeformState::enter(button, window, mousePos);
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
