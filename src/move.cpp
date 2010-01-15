#include <windows.h>

#include "move.hpp"
#include "debug.hpp"

MoveWorker::MoveWorker(POINT const &mousePos, HWND window)
:
	DeformWorker(mousePos, window)
{
	DEBUGLOG("Starting move action");

	d_parent = GetAncestor(d_window, GA_PARENT);

	RECT rect;
	GetWindowRect(d_window, &rect);
	d_windowPos.x = rect.left;
	d_windowPos.y = rect.top;

	d_cursorWindow.setCursor(crMove);
}

void MoveWorker::move(POINT const &mousePos) {
	DeformWorker::move(mousePos);

	d_windowPos.x += d_mouseDelta.x;
	d_windowPos.y += d_mouseDelta.y;
	moveWindow();
}

void MoveWorker::moveWindow() {
	POINT clientPos = d_windowPos;
	if (d_parent) {
		ScreenToClient(d_parent, &clientPos);
	}
	SetWindowPos(d_window, 0, clientPos.x, clientPos.y, 0, 0, SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOZORDER);
}
