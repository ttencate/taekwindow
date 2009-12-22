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

bool ResizeState::updateResizingXY(RECT const &rect) {
	int orx = d_resizingX, ory = d_resizingY;
	switch (globals->config().resizeMode) {
		case rmBottomRight:
			updateResizingBottomRight(rect);
			break;
		case rmNineRectangles:
			updateResizingNineRects(rect);
			break;
	}	
	return orx != d_resizingX || ory != d_resizingY;
}

void ResizeState::updateResizingBottomRight(RECT const &) {
	d_resizingX = 1;
	d_resizingY = 1;
}

void ResizeState::updateResizingNineRects(RECT const &rect) {
	POINT pos = mousePos();
	d_resizingX = rect.right == rect.left ? 0 : (pos.x - rect.left) * 3 / (rect.right - rect.left) - 1;
	d_resizingY = rect.bottom == rect.top ? 0 : (pos.y - rect.top) * 3 / (rect.bottom - rect.top) - 1;
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
	return crNormal;
}

/* Sets up the cursor and the resize type.
 */
void ResizeState::enter() {
	DeformState::enter();
	DEBUGLOG("Starting resize action");

	// When resizing a maximized window, unmaximize it first.
	if (IsZoomed(window())) {
		restore();
	}

	d_parent = GetAncestor(window(), GA_PARENT);

	// Find out at which corner to resize.
	RECT rect;
	GetWindowRect(window(), &rect);
	updateResizingXY(rect);

	cursorWindow().setCursor(getResizingCursor());
}

/* Resizes the window accordingly.
 */
bool ResizeState::onMouseMove(MouseMoveEvent const &event) {
	DeformState::onMouseMove(event);

	POINT delta = mouseDelta();

	// Resize at the correct corner/edge.
	RECT rect;
	GetWindowRect(window(), &rect);
	if (updateResizingXY(rect))
		cursorWindow().setCursor(getResizingCursor());
	
	if (d_resizingX < 0) {
		rect.left += delta.x;
	} else if (d_resizingX > 0) {
		rect.right += delta.x;
	}
	if (d_resizingY < 0) {
		rect.top += delta.y;
	} else if (d_resizingY > 0) {
		rect.bottom += delta.y;
	}

	resizeWindow(rect);

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

void ResizeState::resizeWindow(RECT const &rect) {
	DEBUGLOG("Resizing to %d-%d, %d-%d", RECT_ARGS(rect));

	// SetWindowPos takes client coordinates
	POINT pos = { rect.left, rect.top };
	if (d_parent) {
		ScreenToClient(d_parent, &pos);
	}
	SetWindowPos(window(), 0,
		pos.x, pos.y,
		rect.right - rect.left, rect.bottom - rect.top,
		SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER);
	RECT actualRect;
	GetWindowRect(window(), &actualRect);

	// The window may have hit its minimum or maximum size. Move to compensate, if necessary.
	// This does cause some flicker, but I see no reliable way to prevent that.
	int dx = d_resizingX < 0 ? rect.right - actualRect.right : 0;
	int dy = d_resizingY < 0 ? rect.bottom - actualRect.bottom : 0;
	if (dx || dy) {
		SetWindowPos(window(), 0, pos.x + dx, pos.y + dy, 0, 0,
			SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOSIZE);
	}
}
