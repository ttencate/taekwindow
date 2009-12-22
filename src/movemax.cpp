#include "movemax.hpp"
#include "debug.hpp"

MoveMaxState::MoveMaxState(POINT mousePos, MouseButton button, HWND window)
:
	DeformState(mousePos, button, window)
{
}

/* Sets up the cursor.
 */
void MoveMaxState::enter() {
	DeformState::enter();

	DEBUGLOG("Starting maximized move action");
	// Remember the monitor that currently contains the window.
	d_currentMonitor = MonitorFromWindow(window(), MONITOR_DEFAULTTONULL);
	cursorWindow().setCursor(crMove);
}

/* Moves the window accordingly.
 */
bool MoveMaxState::onMouseMove(MouseMoveEvent const &event) {
	DeformState::onMouseMove(event);

	DEBUGLOG("Handling maximized move action");
	HMONITOR mouseMonitor = MonitorFromPoint(event.mousePos, MONITOR_DEFAULTTONEAREST);
	if (mouseMonitor != d_currentMonitor) {
		// Window needs to be moved to another monitor, while retaining its maximized state.
		d_currentMonitor = mouseMonitor;
		// Figure out where the monitor is on the virtual screen.
		MONITORINFO monitorInfo;
		monitorInfo.cbSize = sizeof(monitorInfo);
		GetMonitorInfo(d_currentMonitor, &monitorInfo);
		DEBUGLOG("Monitor work area at %d,%d (%dx%d)", monitorInfo.rcWork.left, monitorInfo.rcWork.top, monitorInfo.rcWork.right - monitorInfo.rcWork.left, monitorInfo.rcWork.bottom - monitorInfo.rcWork.top);
		// Now move the window by unmaximizing, moving, remaximizing.
		// First, lock drawing to prevent annoying flicker.
		LockWindowUpdate(window());
		// Use SetWindowPlacement to change the style to SW_RESTORE, because ShowWindow does animations
		// (simpler ones than AnimateWindow, but still annoying).
		WINDOWPLACEMENT windowPlacement;
		windowPlacement.length = sizeof(windowPlacement);
		GetWindowPlacement(window(), &windowPlacement);
		windowPlacement.showCmd = SW_RESTORE;
		SetWindowPlacement(window(), &windowPlacement);
		// Move the restored window to the top left of the working area of the desired monitor.
		SetWindowPos(window(), 0, monitorInfo.rcWork.left, monitorInfo.rcWork.top, 0, 0, SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOZORDER);
		// Unlock window drawing for the final stage.
		LockWindowUpdate(NULL);
		// And remaximize.
		GetWindowPlacement(window(), &windowPlacement);
		windowPlacement.showCmd = SW_MAXIMIZE;
		SetWindowPlacement(window(), &windowPlacement);
	}
	return true;
}