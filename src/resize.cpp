#include "resize.hpp"
#include "wininfo.hpp"
#include "errors.hpp"
#include "debug.hpp"
#include "globals.hpp"

ResizeWorker::ResizeWorker(POINT const &mousePos, HWND window)
:
	DeformWorker(mousePos, window)
{
	DEBUGLOG("Starting resize action");

	// When resizing a maximized window, unmaximize it first.
	if (IsZoomed(d_window)) {
		restore();
	}

	d_parent = GetAncestor(d_window, GA_PARENT);

	// Find out at which corner to resize.
	RECT rect;
	GetWindowRect(d_window, &rect);
	d_resizingX = d_resizingY = 0;
	updateResizingXY(rect);

	d_cursorWindow.setCursor(getResizingCursor());
}

void ResizeWorker::move(POINT const &mousePos) {
	DeformWorker::move(mousePos);

	// Resize at the correct corner/edge.
	RECT rect;
	GetWindowRect(d_window, &rect);
	if (updateResizingXY(rect))
		d_cursorWindow.setCursor(getResizingCursor());
	
	if (d_resizingX < 0) {
		rect.left += d_mouseDelta.x;
	} else if (d_resizingX > 0) {
		rect.right += d_mouseDelta.x;
	}
	if (d_resizingY < 0) {
		rect.top += d_mouseDelta.y;
	} else if (d_resizingY > 0) {
		rect.bottom += d_mouseDelta.y;
	}

	resizeWindow(rect);
}

bool ResizeWorker::updateResizingXY(RECT const &rect) {
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

void ResizeWorker::updateResizingBottomRight(RECT const &) {
	d_resizingX = 1;
	d_resizingY = 1;
}

void ResizeWorker::updateResizingNineRects(RECT const &rect) {
	POINT pos = d_mousePos;
	int rx = rect.right == rect.left ? 0 : (pos.x - rect.left) * 3 / (rect.right - rect.left) - 1;
	int ry = rect.bottom == rect.top ? 0 : (pos.y - rect.top) * 3 / (rect.bottom - rect.top) - 1;
	if (rx)
		d_resizingX = rx;
	if (ry)
		d_resizingY = ry;
}

/* Returns the cursor to be used for the current resizing direction.
 */
Cursor ResizeWorker::getResizingCursor() {
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

/* Set its restored size to its maximized size, but pull the borders onto the screen.
 */
void ResizeWorker::restore() {
	WINDOWPLACEMENT windowPlacement;
	windowPlacement.length = sizeof(WINDOWPLACEMENT);
	if (!GetWindowPlacement(d_window, &windowPlacement)) {
		debugShowLastError("GetWindowPlacement");
		return;
	}
	DEBUGLOG("Current window restored position: %d, %d, %d, %d",
		windowPlacement.rcNormalPosition.left, windowPlacement.rcNormalPosition.top,
		windowPlacement.rcNormalPosition.right, windowPlacement.rcNormalPosition.bottom);

	// A complication: Get/SetWindowPlacement use workspace coordinates, not screen coordinates.
	// The difference is that workspace coordinates exclude the taskbar.
	// TODO test this on multi-monitor
	HMONITOR monitor = MonitorFromWindow(d_window, MONITOR_DEFAULTTONEAREST);
	MONITORINFO monInfo;
	monInfo.cbSize = sizeof(MONITORINFO);
	if (!GetMonitorInfo(monitor, &monInfo)) {
		debugShowLastError("GetMonitorInfo");
		return;
	}
	DEBUGLOG("Monitor work area: %d, %d, %d, %d",
		monInfo.rcWork.left, monInfo.rcWork.top, monInfo.rcWork.right, monInfo.rcWork.bottom);

	windowPlacement.length = sizeof(WINDOWPLACEMENT);
	windowPlacement.showCmd = SW_RESTORE;
	windowPlacement.rcNormalPosition.left = 0;
	windowPlacement.rcNormalPosition.top = 0;
	windowPlacement.rcNormalPosition.right = monInfo.rcWork.right - monInfo.rcWork.left;
	windowPlacement.rcNormalPosition.bottom = monInfo.rcWork.bottom - monInfo.rcWork.top;
	DEBUGLOG("Setting restored position to %d, %d, %d, %d",
		windowPlacement.rcNormalPosition.left, windowPlacement.rcNormalPosition.top,
		windowPlacement.rcNormalPosition.right, windowPlacement.rcNormalPosition.bottom);

	// Use SetWindowPlacement for demaximizing to prevent animation.
	if (!SetWindowPlacement(d_window, &windowPlacement))
		debugShowLastError("SetWindowPlacement");

	// TODO what was this good for, again?
	SendMessage(d_window, WM_NULL, 0, 0);
}

void ResizeWorker::resizeWindow(RECT const &rect) {
	// SetWindowPos takes client coordinates
	POINT pos = { rect.left, rect.top };
	if (d_parent) {
		ScreenToClient(d_parent, &pos);
	}
	SetWindowPos(d_window, 0,
		pos.x, pos.y,
		rect.right - rect.left, rect.bottom - rect.top,
		SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER);
	RECT actualRect;
	GetWindowRect(d_window, &actualRect);

	// The window may have hit its minimum or maximum size. Move to compensate, if necessary.
	// This does cause some flicker, but I see no reliable way to prevent that.
	int dx = d_resizingX < 0 ? rect.right - actualRect.right : 0;
	int dy = d_resizingY < 0 ? rect.bottom - actualRect.bottom : 0;
	if (dx || dy) {
		SetWindowPos(d_window, 0, pos.x + dx, pos.y + dy, 0, 0,
			SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOSIZE);
	}
}
