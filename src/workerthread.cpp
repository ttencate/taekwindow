#include <windows.h>
#include <windowsx.h>
#include <tchar.h>

#include "workerthread.hpp"
#include "globals.hpp"
#include "errors.hpp"

// We want to use this, but it requires WIN32_WINNT to be 0x600 at least (Windows Vista).
// By setting that, we'd risk accidentally using other Vista features,
// damaging XP support... so we just copy the definition here.
#define WM_MOUSEHWHEEL 0x020E

WorkerThread::WorkerThread()
:
	d_id(0),
	d_exiting(false)
{
}

WorkerThread::~WorkerThread() {
	if (d_id)
		stop();
}

void WorkerThread::start() {
	if (!CreateThread(NULL, 0, &staticThreadProc, this, 0, &d_id)) {
		showLastError(NULL, _T("CreateThread failed"));
		return;
	}
}

void WorkerThread::stop() {
	d_exiting = true;
}

void WorkerThread::postMessage(UINT message, WPARAM wParam, LPARAM lParam) {
	PostThreadMessage(d_id, message, wParam, lParam);
}

DWORD WINAPI WorkerThread::staticThreadProc(LPVOID param) {
	return ((WorkerThread*)param)->threadProc();
}

DWORD WorkerThread::threadProc() {
	while (!d_exiting) {
		MSG msg;
		BOOL ret = GetMessage(&msg, NULL, 0, 0);
		if (ret == -1) {
			showLastError(NULL, _T("GetMessage failed"));
			return 1;
		}
		if (ret == 0) {
			break;
		}
		if (msg.hwnd != NULL || !handleMessage(msg.message, msg.wParam, msg.lParam)) {
			DispatchMessage(&msg);
		}
	}
	return 0;
}

bool WorkerThread::handleMessage(UINT message, WPARAM wParam, LPARAM lParam) {
	// TODO this doesn't yet handle keyboard messages; those have different wparam/lparam
	POINT mousePos;
	mousePos.x = GET_X_LPARAM(lParam);
	mousePos.y = GET_Y_LPARAM(lParam);
	HWND window = WindowFromPoint(mousePos);
	switch (message) {
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
			//DEBUGLOG("Worker thread handling mouse down 0x%08x at (%i, %i)", message, mousePos.x, mousePos.y);
			globals->mouseHandlerList().onMouseDown(MouseDownEvent(mousePos, eventToButton(message), window));
			return true;
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
			//DEBUGLOG("Worker thread handling mouse up 0x%08x at (%i, %i)", message, mousePos.x, mousePos.y);
			globals->mouseHandlerList().onMouseUp(MouseUpEvent(mousePos, eventToButton(message), window));
			return true;
		case WM_MOUSEMOVE:
			collapseMoves(&mousePos);
			//DEBUGLOG("Worker thread handling mouse move 0x%08x at (%i, %i)", message, mousePos.x, mousePos.y);
			globals->mouseHandlerList().onMouseMove(MouseMoveEvent(mousePos));
			return true;
		case WM_MOUSEWHEEL:
		case WM_MOUSEHWHEEL:
			//DEBUGLOG("Worker thread handling mouse wheel 0x%08x at (%i, %i) with wParam 0x%08x", message, mousePos.x, mousePos.y, wParam);
			globals->mouseHandlerList().onMouseWheel(MouseWheelEvent(wParam, mousePos, wParam, window));
			return true;
		default:
			return false;
	}
}

void WorkerThread::collapseMoves(POINT *mousePos) {
	MSG msg;
	while (PeekMessage(&msg, NULL, WM_MOUSEMOVE, WM_MOUSEMOVE, PM_REMOVE)) {
		mousePos->x = GET_X_LPARAM(msg.lParam);
		mousePos->y = GET_Y_LPARAM(msg.lParam);
	}
}
