#include "deform.hpp"
#include "debug.hpp"

DeformWorker::DeformWorker(POINT mousePos, HWND window)
:
	d_mousePos(mousePos),
	d_window(window)
{
	d_mouseDelta.x = d_mouseDelta.y = 0;
}

void DeformWorker::move(POINT const &mousePos) {
	d_mouseDelta.x = mousePos.x - d_mousePos.x;
	d_mouseDelta.y = mousePos.y - d_mousePos.y;
	d_mousePos = mousePos;
}
