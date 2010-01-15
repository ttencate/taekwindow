#include "movemax.hpp"
#include "debug.hpp"

MoveMaxWorker::MoveMaxWorker(POINT const &mousePos, HWND window)
:
	DeformWorker(mousePos, window),
	d_currentMonitor(MonitorFromWindow(window, MONITOR_DEFAULTTONULL))
{
	DEBUGLOG("Starting maximized move action");
	d_cursorWindow.setCursor(crMove);
}

void MoveMaxWorker::move(POINT const &mousePos) {
	DeformWorker::move(mousePos);

	HMONITOR mouseMonitor = MonitorFromPoint(mousePos, MONITOR_DEFAULTTONEAREST);
	if (mouseMonitor != d_currentMonitor) {
		moveToMonitor(mouseMonitor);
		d_currentMonitor = mouseMonitor;
	}
}

void MoveMaxWorker::moveToMonitor(HMONITOR monitor) {
	// Figure out where the monitor is on the virtual screen.
	MONITORINFO monitorInfo;
	monitorInfo.cbSize = sizeof(monitorInfo);
	GetMonitorInfo(monitor, &monitorInfo);
	DEBUGLOG("Monitor work area at %d,%d (%dx%d)", monitorInfo.rcWork.left, monitorInfo.rcWork.top, monitorInfo.rcWork.right - monitorInfo.rcWork.left, monitorInfo.rcWork.bottom - monitorInfo.rcWork.top);
	
	// Now move the window by unmaximizing, moving, remaximizing.

	// First, lock drawing to prevent annoying flicker.
	LockWindowUpdate(d_window);
	// Use SetWindowPlacement to change the style to SW_RESTORE, because ShowWindow does animations
	// (simpler ones than AnimateWindow, but still annoying).
	WINDOWPLACEMENT windowPlacement;
	windowPlacement.length = sizeof(windowPlacement);
	GetWindowPlacement(d_window, &windowPlacement);
	windowPlacement.showCmd = SW_RESTORE;
	SetWindowPlacement(d_window, &windowPlacement);

	// Move the restored window to the top left of the working area of the desired monitor.
	SetWindowPos(d_window, 0, monitorInfo.rcWork.left, monitorInfo.rcWork.top, 0, 0, SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOZORDER);

	// Unlock window drawing for the final stage.
	LockWindowUpdate(NULL);
	// And remaximize.
	GetWindowPlacement(d_window, &windowPlacement);
	windowPlacement.showCmd = SW_MAXIMIZE;
	SetWindowPlacement(d_window, &windowPlacement);
}