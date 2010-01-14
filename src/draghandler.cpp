#include "draghandler.hpp"
#include "globals.hpp"
#include "messages.hpp"
#include "winutils.hpp"
#include "wininfo.hpp"
#include "debug.hpp"
#include "move.hpp"
#include "movemax.hpp"
#include "resize.hpp"
#include "ignore.hpp"

DragHandler::DragHandler()
:
	d_draggingButton(mbNone),
	d_worker(NULL)
{
}

DragHandler::~DragHandler() {
	delete d_worker;
}

bool DragHandler::llMouseDown(LLMouseDownEvent const &event) {
	if (d_draggingButton != mbNone) {
		return true;
	}
	if (isModifierDown()) {
		if (event.button == globals->config().moveButton || event.button == globals->config().resizeButton) {
			DEBUGLOG("Hook starting a drag action");
			d_draggingButton = event.button;
			globals->workerThread().postMessage(DRAG_START_MESSAGE, (WPARAM)event.button, pointToDword(event.mousePos));
			return true;
		}
	}
	return false;
}

bool DragHandler::llMouseUp(LLMouseUpEvent const &event) {
	if (d_draggingButton != mbNone) {
		if (d_draggingButton == event.button) {
			DEBUGLOG("Hook ending a drag action");
			d_draggingButton = mbNone;
			globals->workerThread().postMessage(DRAG_END_MESSAGE, (WPARAM)event.button, pointToDword(event.mousePos));
		}
		return true;
	}
	return false;
}

bool DragHandler::llMouseMove(LLMouseMoveEvent const &event) {
	if (d_draggingButton == mbNone) {
		return false;
	} else {
		globals->workerThread().postMessage(DRAG_MOVE_MESSAGE, 0, pointToDword(event.mousePos));
		return true;
	}
}

bool DragHandler::handleMessage(UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case DRAG_START_MESSAGE:
			handleDragStart((MouseButton)wParam, dwordToPoint(lParam));
			return true;
		case DRAG_END_MESSAGE:
			handleDragEnd((MouseButton)wParam, dwordToPoint(lParam));
			return true;
		case DRAG_MOVE_MESSAGE:
			handleDragMove(dwordToPoint(lParam));
			return true;
	}
	return false;
}

bool DragHandler::isModifierDown() const {
	return (GetAsyncKeyState(globals->config().modifier) & 0x8000) != 0;
}

void DragHandler::handleDragStart(MouseButton button, POINT const &mousePos) {
	DEBUGLOG("Worker starting a drag action");
	if (button == globals->config().moveButton) {
		handleMoveStart(mousePos);
	} else if (button == globals->config().resizeButton) {
		handleResizeStart(mousePos);
	}
}

void DragHandler::handleMoveStart(POINT const &mousePos) {
	// We prefer windows that are not maximized over those that are, which makes sense in an MDI environment.
	// This would be what the user expected.
	HWND window = WindowFromPoint(mousePos);
	HWND parentWindow = findFirstParent(window, isRestoredMovableWindow);
	if (parentWindow) {
		d_worker = new MoveWorker(mousePos, parentWindow);
		return;
	} else {
		// No unmaximized movable window found; look for a maximized one that can be kicked to another monitor.
		// Only top-level windows can be moved to other monitors, I guess.
		// So prefer the outermost (last) parent that is maximized.
		parentWindow = findLastParent(window, isMaximizedMovableWindow);
		if (parentWindow) {
			d_worker = new MoveMaxWorker(mousePos, parentWindow);
			return;
		}
	}
	d_worker = new IgnoreWorker();
}

void DragHandler::handleResizeStart(POINT const &mousePos) {
	// Try to find a parent window that we can resize without unmaximizing.
	// This one is probably the one that the user meant.
	HWND window = WindowFromPoint(mousePos);
	HWND parentWindow = findFirstParent(window, isRestoredResizableWindow);
	if (parentWindow) {
		d_worker = new ResizeWorker(mousePos,  parentWindow);
		return;
	} else {
		// No unmaximized window; perhaps we have a maximized one?
		// We prefer the outermost, so that a maximized MDI parent is preferred over a maximized MDI child inside it.
		// This makes the most sense, because users often forget that there is an MDI at all when their MDI childs are maximized.
		parentWindow = findLastParent(window, isMaximizedResizableWindow);
		if (parentWindow) {
			d_worker = new ResizeWorker(mousePos,  parentWindow);
			return;
		}
	}
	d_worker = new IgnoreWorker();
}

void DragHandler::handleDragEnd(MouseButton, POINT const &) {
	DEBUGLOG("Worker ending a drag action");
	delete d_worker;
	d_worker = NULL;
}

void DragHandler::handleDragMove(POINT const &mousePos) {
	d_worker->move(mousePos);
}

