#include "drag.hpp"
#include "config.hpp"
#include "state.hpp"

/* Sets the variables resizingX and resizingY to the proper values,
 * considering the screen-coordinate pointer location.
 */
void setResizingX(POINT const &pt) {
	resizingX = (pt.x - lastRect.left) * 3 / (lastRect.right - lastRect.left) - 1;
}
void setResizingY(POINT const &pt) {
	resizingY = (pt.y - lastRect.top) * 3 / (lastRect.bottom - lastRect.top) - 1;
}

void startDragAction(MouseButton button, MOUSEHOOKSTRUCT const *eventInfo) {
	// Capture the mouse so it'll still get events even if the mouse leaves the window
	// (could happen while resizing).
	SetCapture(draggedWindow);
	GetWindowRect(draggedWindow, &lastRect);
	if (button == resizeButton) {
		switch (resizeMode) {
			case rmBottomRight:
				resizingX = 1;
				resizingY = 1;
				break;
			case rmNineRectangles:
				// Figure out in which area we're dragging to resize in the proper direction.
				setResizingX(eventInfo->pt);
				setResizingY(eventInfo->pt);
				break;
		}
	}
}

void doDragAction(MOUSEHOOKSTRUCT const *eventInfo) {
	int deltaX, deltaY;
	// Find out the movement since the last known mouse position.
	deltaX = eventInfo->pt.x - lastMousePos.x, deltaY = eventInfo->pt.y - lastMousePos.y;
	if (draggingButton == moveButton) {
		lastRect.left += deltaX;
		lastRect.top += deltaY;
		lastRect.right += deltaX;
		lastRect.bottom += deltaY;
	} else if (draggingButton == resizeButton) {
		// Resize at the right corner/edge.
		switch (resizingX) {
			case -1:
				lastRect.left += deltaX; break;
			case 1:
				lastRect.right += deltaX; break;
			case 0:
				// We may have come close to a vertical border in the meantime.
				setResizingX(eventInfo->pt); break;
		}
		switch (resizingY) {
			case -1:
				lastRect.top += deltaY; break;
			case 1:
				lastRect.bottom += deltaY; break;
			case 0:
				// We may have come close to a horizontal border in the meantime.
				setResizingY(eventInfo->pt); break;
		}
	}
	// Now possibly the most important statement in the program: apply the movement/resizing to the window!
	SetWindowPos(draggedWindow, NULL,
		lastRect.left, lastRect.top, lastRect.right - lastRect.left, lastRect.bottom - lastRect.top,
		SWP_NOACTIVATE | SWP_NOOWNERZORDER);
}

void endDragAction() {
	ReleaseCapture();
	currentState = dsNone;
}
