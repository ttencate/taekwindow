#include "resize.hpp"
#include "wininfo.hpp"
#include "errors.hpp"
#include "debug.hpp"
#include "globals.hpp"

// TODO remove
#define RECT_ARGS(r) r.left, r.right, r.top, r.bottom

ResizeState::ResizeState(POINT mousePos, MouseButton button, HWND window)
:
	DeformState(mousePos, button, window)
{
}

bool ResizeState::updateResizingXY() {
	int orx = d_resizingX, ory = d_resizingY;
	switch (globals->config().resizeMode) {
		case rmBottomRight:
			updateResizingBottomRight();
			break;
		case rmNineRectangles:
			updateResizingNineRects();
			break;
	}	
	return orx != d_resizingX || ory != d_resizingY;
}

void ResizeState::updateResizingBottomRight() {
	d_resizingX = 1;
	d_resizingY = 1;
}

void ResizeState::updateResizingNineRects() {
	POINT clientPos = mousePos();
	ScreenToClient(window(), &clientPos);
	d_resizingX = clientPos.x * 3 / (d_actualRect.right - d_actualRect.left) - 1;
	d_resizingY = clientPos.y * 3 / (d_actualRect.bottom - d_actualRect.top) - 1;
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

	// When resizing a maximized window, unmaximize it first.
	if (IsZoomed(window())) {
		restore();
		DEBUGLOG("Returned from restore");
	}
	DEBUGLOG("After the if");

	d_parent = GetAncestor(window(), GA_PARENT);
	DEBUGLOG("Got the ancestor");
	d_actualRect = currentRect();
	DEBUGLOG("Returned from currentRect");
	DEBUGLOG("Actual rect is %d-%d, %d-%d", RECT_ARGS(d_actualRect));
	
	d_desiredRect = d_actualRect;
	DEBUGLOG("Desired rect is %d-%d, %d-%d", RECT_ARGS(d_desiredRect));

	// Find out at which corner to resize.
	updateResizingXY();
	DEBUGLOG("Updated resizingXY");

	cursorWindow().setCursor(getResizingCursor());
	DEBUGLOG("Cursor set; returning from enter");
}

/* Resizes the window accordingly.
 */
bool ResizeState::onMouseMove(MouseMoveEvent const &event) {
	DeformState::onMouseMove(event);

	POINT delta = mouseDelta();

	// Resize at the correct corner/edge.
	if (d_resizingX < 0) {
		d_desiredRect.left += delta.x;
	} else if (d_resizingX > 0) {
		d_desiredRect.right += delta.x;
	}
	if (d_resizingY < 0) {
		d_desiredRect.top += delta.y;
	} else if (d_resizingY > 0) {
		d_desiredRect.bottom += delta.y;
	}

	resizeWindow();
	d_actualRect = currentRect();

	if (updateResizingXY())
		cursorWindow().setCursor(getResizingCursor());
	
	return true;
}

/* Set its restored size to its maximized size, but pull the borders onto the screen.
 */
void ResizeState::restore() {
	WINDOWPLACEMENT windowPlacement;
	windowPlacement.length = sizeof(WINDOWPLACEMENT);
	if (!GetWindowPlacement(window(), &windowPlacement))
		debugShowLastError("GetWindowPlacement");

	// A complication: Get/SetWindowPlacement use workspace coordinates, not screen coordinates.
	// The difference is that workspace coordinates exclude the taskbar.
	// TODO test this on multi-monitor
	HMONITOR monitor = MonitorFromWindow(window(), MONITOR_DEFAULTTONEAREST);
	MONITORINFO monInfo;
	monInfo.cbSize = sizeof(MONITORINFO);
	if (!GetMonitorInfo(monitor, &monInfo))
		debugShowLastError("GetMonitorInfo");

	ASSERT(windowPlacement.length == sizeof(WINDOWPLACEMENT));
	windowPlacement.showCmd = SW_RESTORE;
	windowPlacement.rcNormalPosition.left = 0;
	windowPlacement.rcNormalPosition.top = 0;
	windowPlacement.rcNormalPosition.right = monInfo.rcWork.right - monInfo.rcWork.left;
	windowPlacement.rcNormalPosition.bottom = monInfo.rcWork.bottom - monInfo.rcWork.top;
	DEBUGLOG("Restoring 0x%08x to %d-%d, %d-%d", window(), RECT_ARGS(windowPlacement.rcNormalPosition));

	// Use SetWindowPlacement for demaximizing to prevent animation.
	if (!SetWindowPlacement(window(), &windowPlacement))
		debugShowLastError("SetWindowPlacement");

	DEBUGLOG("Restored 0x%08x to %d-%d, %d-%d", window(), RECT_ARGS(windowPlacement.rcNormalPosition));
}

void ResizeState::resizeWindow() {
	DEBUGLOG("Resizing to %d-%d, %d-%d", RECT_ARGS(d_desiredRect));
	// Reminder: SetWindowPos takes client coordinates
	SetWindowPos(window(), 0,
		d_desiredRect.left, d_desiredRect.top,
		d_desiredRect.right - d_desiredRect.left, d_desiredRect.bottom - d_desiredRect.top,
		SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER);
}

RECT ResizeState::currentRect() const {
	RECT rect;
	GetWindowRect(window(), &rect);
	if (d_parent) {
		screenToClient(d_parent, rect);
	}
	DEBUGLOG("Current is %d-%d, %d-%d", RECT_ARGS(rect));
	return rect;
}
