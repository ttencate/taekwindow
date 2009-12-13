#include "resize.hpp"
#include "debuglog.hpp"
#include "main.hpp"

ResizeState::ResizeState(POINT mousePos, MouseButton button, HWND window)
:
	DeformState(mousePos, button, window)
{
}

void ResizeState::setResizingX(POINT const &pt) {
	d_resizingX = pt.x * 3 / (lastRect().right - lastRect().left) - 1;
}

void ResizeState::setResizingY(POINT const &pt) {
	d_resizingY = pt.y * 3 / (lastRect().bottom - lastRect().top) - 1;
}

/* Returns the cursor to be used for the current resizing direction.
 */
Cursor ResizeState::getResizingCursor() {
	if (d_resizingX && !d_resizingY)
		return crResizeWE;
	if (!d_resizingX && d_resizingY)
		return crResizeNS;
	if (d_resizingX * d_resizingY > 0)
		return crResizeNWSE;
	if (d_resizingX * d_resizingY < 0)
		return crResizeNESW;
	return crNormal; // fallback
}

/* Sets up the cursor and the resize type.
 */
void ResizeState::enter() {
	DeformState::enter();

	DEBUGLOG("Starting resize action");
	if (IsZoomed(window())) {
		// When resizing a maximized window, unmaximize it first.
		// Set its restored size to its maximized size, but pull the borders onto the screen.
		// When a window is maximized, it is actually slightly off the screen on all sides, to hide its borders.
		RECT rect;
		GetWindowRect(window(), &rect);
		WINDOWINFO windowInfo;
		windowInfo.cbSize = sizeof(windowInfo);
		GetWindowInfo(window(), &windowInfo);
		rect.left += windowInfo.cxWindowBorders;
		rect.top += windowInfo.cyWindowBorders;
		rect.right -= windowInfo.cxWindowBorders;
		rect.bottom -= windowInfo.cyWindowBorders;
		// Use SetWindowPlacement for demaximizing to prevent animation.
		// And while we're at it, set the size too.
		WINDOWPLACEMENT windowPlacement;
		windowPlacement.length = sizeof(windowPlacement);
		GetWindowPlacement(window(), &windowPlacement);
		windowPlacement.showCmd = SW_RESTORE;
		windowPlacement.rcNormalPosition = lastRect();
		SetWindowPlacement(window(), &windowPlacement);
	}
	// Find out at which corner to resize.
	POINT clientPos = mousePos();
	ScreenToClient(window(), &clientPos);
	switch (activeConfig.resizeMode) {
		case rmBottomRight:
			d_resizingX = 1;
			d_resizingY = 1;
			break;
		case rmNineRectangles:
			// Figure out in which area we're dragging to resize in the proper direction.
			setResizingX(clientPos);
			setResizingY(clientPos);
			break;
	}
	cursorWindow().setCursor(getResizingCursor());
}

/* Resizes the window accordingly.
 */
bool ResizeState::onMouseMove(MouseMoveEvent const &event) {
	DeformState::onMouseMove(event);

	DEBUGLOG("Handling resize action");
	POINT delta = mouseDelta();
	// Do not move the window, unless resizing at its top and/or its left.
	UINT flags = SWP_NOMOVE;
	// Resize at the right corner/edge.
	POINT clientPos = event.mousePos;
	ScreenToClient(window(), &clientPos);
	RECT rect = lastRect();
	bool needCursorUpdate = false;
	switch (d_resizingX) {
		case -1:
			rect.left += delta.x;
			flags &= ~SWP_NOMOVE;
			break;
		case 1:
			rect.right += delta.x;
			break;
		case 0:
			// We may have come close to a vertical border in the meantime.
			setResizingX(clientPos);
			if (d_resizingX)
				needCursorUpdate = true;
			break;
	}
	switch (d_resizingY) {
		case -1:
			rect.top += delta.y;
			flags &= ~SWP_NOMOVE;
			break;
		case 1:
			rect.bottom += delta.y;
			break;
		case 0:
			// We may have come close to a horizontal border in the meantime.
			setResizingY(clientPos);
			if (d_resizingY)
				needCursorUpdate = true;
			break;
	}
	if (needCursorUpdate)
		cursorWindow().setCursor(getResizingCursor());
	updateWindowPos(rect, flags);
	return true;
}
