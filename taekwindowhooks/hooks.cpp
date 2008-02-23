#include <windows.h>

#include "hooks.hpp"
#include "drag.hpp"
#include "actions.hpp"
#include "config.hpp"
#include "util.hpp"
#include "debuglog.hpp"

/* The button that we're dragging with.
 * Only meaningful while we're dragging, of course.
 */
extern MouseButton draggingButton;

/* Whether or not the modifier key is currently down.
 */
extern bool modifierDown;

/* The current state we're in.
 */
extern DragState currentState;

/* Whether or not dragging has occurred since the last key-down event of the Modifier.
 * If dragging has occurred, this prevents the key-up event to be passed on.
 */
extern bool haveDragged;

#ifdef _DEBUG
/* Steal this from main.cpp.
 */
extern DWORD mainThreadId;
#endif

/* Handles a button-down event for the move and resize buttons.
 * Assumes that button is the move or resize button, that the modifier is down, and that dragState is dsNone.
 */
void handleDragStart(MouseButton button, HWND window, POINT mousePos) {
	DEBUGLOG("Handling button down event");
	// Store the button we're using so we know when it's released.
	draggingButton = button;
	// Remember that we have dragged something during this press of the modifier, which means that
	// the subsequent release event should be eaten.
	haveDragged = true;
	// Yippee! A Modifier-drag event just started that we want to process (or ignore).
	// Find the actual window being dragged: this is the top-level window that is the ultimate parent
	// of the window receiving the event. Seems to work for MDI's too.
	if (button == config.moveButton) {
		// Try to find movable ancestor.
		window = findGrabbedParent(window, false);
		if (window) {
			currentState = dsMoving;
			startMoveAction(window, mousePos);
		} else {
			DEBUGLOG("Ignoring button down event because no movable parent was found", window);
			currentState = dsIgnoring;
		}
	} else if (button == config.resizeButton) {
		// Try to find resizable ancestor.
		window = findGrabbedParent(window, true);
		if (window) {
			currentState = dsResizing;
			startResizeAction(window, mousePos);
		} else {
			DEBUGLOG("Ignoring button down event because no resizable parent was found", window);
			currentState = dsIgnoring;
		}
	}
}

/* Handles a mouse button release event for the button we're currently dragging with.
 * Assumes that button is draggingButton.
 * Returns true if the event was processed and should not be passed to the application, false otherwise.
 */
void handleDragEnd(MouseButton button, POINT mousePos) {
	DEBUGLOG("Handling button up event");
	// The button we're dragging with was released.
	draggingButton = mbNone;
	switch (currentState) {
		case dsMoving:
			endMoveAction();
			currentState = dsNone;
			return;
		case dsResizing:
			endResizeAction();
			currentState = dsNone;
			return;
		case dsIgnoring:
			DEBUGLOG("Ending the ignoring");
			currentState = dsNone;
			return;
	}
}

/* Handles a mouse movement event.
 * Assumes that dragState is not dsNone; i.e. we are dragging something.
 */
void handleDrag(POINT mousePos) {
	if (currentState == dsMoving) {
		// We are handling the moving or resizing of a window.
		doMoveAction(mousePos);
	} else if (currentState == dsResizing) {
		doResizeAction(mousePos);
	}
}

/* Handles a "push window to the background" event.
 */
void handlePushBack(HWND window) {
	window = findGrabbedParent(window, false);
	doPushBack(window);
}

/* The function for handling mouse events. This is the reason why we have to use a separate DLL;
 * see the SetWindowsHookEx documentation for details.
 */
LRESULT CALLBACK mouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
	DEBUGLOG("Mouse hook called");
	bool processed = false; // Set to true if we don't want to pass the event to the application.
	if (nCode >= 0 && nCode == HC_ACTION) { // If nCode < 0, do nothing as per Microsoft's recommendations.
		MOUSEHOOKSTRUCT *eventInfo = (MOUSEHOOKSTRUCT*)lParam;
		MouseButton button;
		switch (wParam) {
			case WM_LBUTTONDOWN:
			case WM_MBUTTONDOWN:
			case WM_RBUTTONDOWN:
			case WM_NCLBUTTONDOWN:
			case WM_NCMBUTTONDOWN:
			case WM_NCRBUTTONDOWN:
				button = eventToButton(wParam);
				// Do we want to start handling a drag event?
				if (modifierDown && currentState == dsNone && (button == config.moveButton || button == config.resizeButton)) {
					handleDragStart(button, eventInfo->hwnd, eventInfo->pt);
					processed = true;
					break;
				}
				// Are we pushing the window to the back?
				if (!modifierDown && button == config.pushBackButton && eventInfo->wHitTestCode == HTCAPTION) {
					handlePushBack(eventInfo->hwnd);
				}
				break;
			case WM_LBUTTONUP:
			case WM_MBUTTONUP:
			case WM_RBUTTONUP:
			case WM_NCLBUTTONUP:
			case WM_NCMBUTTONUP:
			case WM_NCRBUTTONUP:
				button = eventToButton(wParam);
				// We only want to take action if it's the current dragging button being released.
				if (button == draggingButton) {
					handleDragEnd(button, eventInfo->pt);
					processed = true;
					break;
				}
				break;
			case WM_MOUSEMOVE:
			case WM_NCMOUSEMOVE:
				if (currentState != dsNone) {
					handleDrag(eventInfo->pt);
					processed = true;
				}
				break;
		}
	}

	LRESULT res = CallNextHookEx((HHOOK)37, nCode, wParam, lParam); // first argument ignored
	if (processed)
		res = 1; // nonzero return value prevents passing the event to the application
	return res;
}

bool isModifier(DWORD vkCode) {
	if (vkCode == config.modifier)
		return true;
	if (config.modifier == VK_MENU && (vkCode == VK_LMENU || vkCode == VK_RMENU))
		return true;
	if (config.modifier == VK_SHIFT && (vkCode == VK_LSHIFT || vkCode == VK_RSHIFT))
		return true;
	if (config.modifier == VK_CONTROL && (vkCode == VK_LCONTROL || vkCode == VK_RCONTROL))
		return true;
	return false;
}

/* The function for handling keyboard events, tracking the state of the modifier key(s).
 * Also the function to eat keyboard events that the application shouldn't receive.
 * Note that this runs in the context of the main exe.
 */
LRESULT CALLBACK lowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	DEBUGLOG("Keyboard hook called");
	if (nCode >= 0 && nCode == HC_ACTION) { // A little redundant, yes. But the docs say it.
		KBDLLHOOKSTRUCT *info = (KBDLLHOOKSTRUCT*)lParam;
		DEBUGLOG("vkCode = 0x%08X, flags = 0x%08X", info->vkCode, info->flags);
#ifdef _DEBUG
		if (info->vkCode == 0x51) {
			DEBUGLOG("Panic button pressed");
			// Q button pressed. Panic button for debugging.
			PostThreadMessage(mainThreadId, WM_QUIT, 0, 0);
			return 1;
		}
#endif
		if (isModifier(info->vkCode)) {
			// Something MAY have happened to the modifier key, but this could also be a repeat.
			bool wasDown = modifierDown;
			modifierDown = !(info->flags & LLKHF_UP);
			DEBUGLOG("Modifier going from %i to %i", wasDown, modifierDown);
			if (wasDown && !modifierDown) {
				DEBUGLOG("Modifier released");
				// Modifier was released. Only pass the event on if there was no drag event.
				/* If we do this bit of code, the app will sometimes still think Alt is down. Test e.g. in Photoshop.
				   This happens especially when both Alt keys are modifiers and we use them interchangably.
				   I have not yet found a reliable way to reproduce this behaviour.
				if (haveDragged) {
					DEBUGLOG("Eating modifier up event");
					return 1;
				}
				*/
				haveDragged = false;
			} else if (!wasDown && modifierDown) {
				DEBUGLOG("Modifier pressed");
				// Modifier was pressed. There has been no drag event since.
				haveDragged = false;
			}
		}
	}
	return CallNextHookEx((HHOOK)37, nCode, wParam, lParam); // first argument ignored
}
