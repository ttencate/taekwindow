#include <windows.h>
#include <windowsx.h>
#include <tchar.h>

#include "workerthread.hpp"
#include "globals.hpp"
#include "errors.hpp"
#include "winutils.hpp"
#include "messages.hpp"

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
		if (msg.hwnd == NULL) {
			if (msg.message == DRAG_MOVE_MESSAGE)
				collapseMoves(&msg);
			globals->mouseHandlerList().handleMessage(msg.message, msg.wParam, msg.lParam);
		} else {
			DispatchMessage(&msg);
		}
	}
	return 0;
}

void WorkerThread::collapseMoves(MSG *msg) {
	// Collapse subsequent move messages into one to give a huge speedup when resizing.
	while (PeekMessage(msg, NULL, DRAG_MOVE_MESSAGE, DRAG_MOVE_MESSAGE, PM_REMOVE))
		;
}
