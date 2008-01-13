#include <windows.h>

#include "hooks.hpp"
#include "drag.hpp"
#include "config.hpp"
#include "util.hpp"
#include "state.hpp"

/* Processes a button-down event.
 * Returns true if the event should not be passed on to the application, false otherwise.
 */
bool processButtonDown(MouseButton button, MOUSEHOOKSTRUCT const *eventInfo) {
	if (currentState == dsNone) {
		// Nothing is yet going on. We possibly want to take action if the Modifier key is currently pressed.
		if (modifierDown) {
			if (button == moveButton || button == resizeButton) {
				// Yippee! A Modifier-drag event just started that we want to process.
				if (button == moveButton)
					currentState = dsMoving;
				else /* button == resizeButton */
					currentState = dsResizing;
				draggingButton = button;
				haveDragged = true;
				// Find the actual window being dragged: this is the top-level window that is the ultimate parent
				// of the window receiving the event. Seems to work for MDI's too.
				draggedWindow = GetAncestor(eventInfo->hwnd, GA_ROOT);
				if (currentState == dsMoving && isMovableWindow(draggedWindow) ||
					currentState == dsResizing && isResizableWindow(draggedWindow)) {
					// Window can be dragged.
					startDragAction(button, eventInfo);
				} else {
					// Modifier-dragging an invalid window. The user won't expect her actions to be passed
					// to that window, so we suppress all events until the mouse button is released.
					currentState = dsIgnoring;
				}
				// Either way, we eat the event.
				return true;
			} else {
				// Mouse-down event with a button we don't handle. Stay away from it.
				return false;
			}
		} else {
			// Modifier was up, so we keep our hands off this event.
			return false;
		}
	} else {
		// We're already dragging, and another button was pressed.
		// Naughty user shouldn't do this, but we pass the event anyway, because otherwise the application
		// might receive a mouse-up event without a preceding mouse-down event and get all confused.
		return false;
	}
}

/* Processes a mouse button release event.
 * Returns true if the event was processed and should not be passed to the application, false otherwise.
 */
bool processButtonUp(MouseButton button, MOUSEHOOKSTRUCT const *eventInfo) {
	switch (currentState) {
		case dsNone:
			// Nothing going on, pass the event on.
			return false;
		case dsMoving:
		case dsResizing:
			if (button == draggingButton) {
				// End of move or resize action.
				// Release the capture and eat the event.
				endDragAction();
				return true;
			} else {
				// Other button released during move event. (Naughty user!)
				return false;
			}
		case dsIgnoring:
			// Ignoring all events until the appropriate button was released.
			if (button == draggingButton) {
				// Well, guess what, it was!
				// But we still ignore this up event, of course.
				currentState = dsNone;
			}
			return true;
		default:
			// Should never be reached unless the currentState enum was tortured by inserting a pineapple into its butt.
			return false;
	}
}

/* Processes a mouse drag event.
 * Returns true if the event was processed and should not be passed to the application, false otherwise.
 */
bool processDrag(MOUSEHOOKSTRUCT *eventInfo) {
	if (currentState == dsMoving || currentState == dsResizing) {
		// We are handling the moving or resizing of a window.
		doDragAction(eventInfo);
		return true;
	}
	return currentState == dsIgnoring;
}

/* The function for handling mouse events. This is the reason why we have to use a separate DLL;
 * see the SetWindowsHookEx documentation for details.
 */
LRESULT CALLBACK mouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
	bool processed = false; // Set to true if we don't want to pass the event to the application.
	if (nCode >= 0 && nCode == HC_ACTION) { // If nCode < 0, do nothing as per Microsoft's recommendations.
		MOUSEHOOKSTRUCT *eventInfo = (MOUSEHOOKSTRUCT*)lParam;
		switch (wParam) {
			case WM_LBUTTONDOWN:
			case WM_MBUTTONDOWN:
			case WM_RBUTTONDOWN:
			case WM_NCLBUTTONDOWN:
			case WM_NCMBUTTONDOWN:
			case WM_NCRBUTTONDOWN:
				processed = processButtonDown(eventToButton(wParam), eventInfo);
				break;
			case WM_LBUTTONUP:
			case WM_MBUTTONUP:
			case WM_RBUTTONUP:
			case WM_NCLBUTTONUP:
			case WM_NCMBUTTONUP:
			case WM_NCRBUTTONUP:
				processed = processButtonUp(eventToButton(wParam), eventInfo);
				break;
			case WM_MOUSEMOVE:
			case WM_NCMOUSEMOVE:
				processed = processDrag(eventInfo);
				break;
		}
		lastMousePos = eventInfo->pt;
	}

	LRESULT res = CallNextHookEx((HHOOK)37, nCode, wParam, lParam); // first argument ignored
	if (processed)
		res = 1; // nonzero return value prevents passing the event to the application
	return res;
}

/* The function for handling keyboard events.
 * Or rather, the function to eat keyboard events that the application shouldn't receive.
 */
LRESULT CALLBACK keyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
#ifdef DEBUG
	if (wParam == 0x51) {
		// Q button pressed. Panic button for debugging.
		PostThreadMessage(mainThreadId, WM_QUIT, 0, 0);
		return 1;
	}
#endif
	if (nCode >= 0 && nCode == HC_ACTION) {
		// Something MAY have happened to the modifier key.
		// We have a key-code in wParam, but it does not distinguish e.g. left and right Alt,
		// because it is VK_MENU for both of them. Therefore we have to use GetAsyncKeyState.
		bool wasDown = modifierDown;
		modifierDown = GetAsyncKeyState(modifier) & 0x8000;
		if (wasDown && !modifierDown) {
			// Modifier was released. Only pass the event on if there was no drag event.
			if (haveDragged) {
				return 1;
			}
			haveDragged = false;
		} else if (!wasDown && modifierDown) {
			// Modifier was pressed. There has been no drag event since.
			haveDragged = false;
		}
	}
	return CallNextHookEx((HHOOK)37, nCode, wParam, lParam); // first argument ignored
}
