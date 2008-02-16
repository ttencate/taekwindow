#include <windows.h>

#include "hooks.hpp"
#include "drag.hpp"
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

/* Handles a button-down event.
 * Returns true if the event should not be passed on to the application, false otherwise.
 */
bool handleButtonDown(MouseButton button, HWND window, POINT mousePos) {
	if (modifierDown && currentState == dsNone && (button == moveButton || button == resizeButton)) {
		DEBUGLOG("Handling button down event");
		// Store the button we're using so we know when it's released.
		draggingButton = button;
		// Remember that we have dragged something during this press of the modifier, which means that
		// the subsequent release event should be eaten.
		haveDragged = true;
		// Yippee! A Modifier-drag event just started that we want to process (or ignore).
		// Find the actual window being dragged: this is the top-level window that is the ultimate parent
		// of the window receiving the event. Seems to work for MDI's too.
		if (button == moveButton) {
			// Try to find movable ancestor.
			window = findGrabbedParent(window, false);
			if (window) {
				currentState = dsMoving;
				startMoveAction(window, mousePos);
			} else {
				DEBUGLOG("Ignoring button down event because no movable parent was found", window);
				currentState = dsIgnoring;
			}
		} else if (button == resizeButton) {
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
		// Either way, we eat the event.
		return true;
	} else {
		// Event we don't handle. Stay away from it.
		CONDDEBUGLOG(!modifierDown, "Ignoring button down event because the modifier is up");
		CONDDEBUGLOG(currentState != dsNone, "Ignoring button down event because the current state is not dsNone");
		CONDDEBUGLOG(button != moveButton && button != resizeButton, "Ignoring button down event because the button is not interesting");
		return false;
	}
}

/* Handles a mouse button release event.
 * Returns true if the event was processed and should not be passed to the application, false otherwise.
 */
bool handleButtonUp(MouseButton button, POINT mousePos) {
	// We only want to take action if it's the current dragging button being released.
	if (button == draggingButton) {
		DEBUGLOG("Handling button up event");
		// The button we're dragging with was released.
		switch (currentState) {
			case dsMoving:
				endMoveAction();
				currentState = dsNone;
				return true;
			case dsResizing:
				endResizeAction();
				currentState = dsNone;
				return true;
			case dsIgnoring:
				DEBUGLOG("Ending the ignoring");
				currentState = dsNone;
				return true;
		}
		draggingButton = mbNone;
	}
	// Nothing interesting.
	return false;
}

/* Handles a mouse movement event.
 * Returns true if the event was processed and should not be passed to the application, false otherwise.
 */
bool handleMove(POINT mousePos) {
	if (currentState == dsMoving) {
		// We are handling the moving or resizing of a window.
		doMoveAction(mousePos);
		return true;
	} else if (currentState == dsResizing) {
		doResizeAction(mousePos);
		return true;
	} else if (currentState == dsIgnoring) {
		return true;
	}
	return false;
}

/* The function for handling mouse events. This is the reason why we have to use a separate DLL;
 * see the SetWindowsHookEx documentation for details.
 */
LRESULT CALLBACK mouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
	DEBUGLOG("Mouse hook called");
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
				processed = handleButtonDown(eventToButton(wParam), eventInfo->hwnd, eventInfo->pt);
				break;
			case WM_LBUTTONUP:
			case WM_MBUTTONUP:
			case WM_RBUTTONUP:
			case WM_NCLBUTTONUP:
			case WM_NCMBUTTONUP:
			case WM_NCRBUTTONUP:
				processed = handleButtonUp(eventToButton(wParam), eventInfo->pt);
				break;
			case WM_MOUSEMOVE:
			case WM_NCMOUSEMOVE:
				processed = handleMove(eventInfo->pt);
				break;
		}
	}

	LRESULT res = CallNextHookEx((HHOOK)37, nCode, wParam, lParam); // first argument ignored
	if (processed)
		res = 1; // nonzero return value prevents passing the event to the application
	return res;
}

bool isModifier(DWORD vkCode) {
	if (vkCode == modifier)
		return true;
	if (modifier == VK_MENU && (vkCode == VK_LMENU || vkCode == VK_RMENU))
		return true;
	if (modifier == VK_SHIFT && (vkCode == VK_LSHIFT || vkCode == VK_RSHIFT))
		return true;
	if (modifier == VK_CONTROL && (vkCode == VK_LCONTROL || vkCode == VK_RCONTROL))
		return true;
	return false;
}

/* The function for handling keyboard events, tracking the state of the modifier key(s).
 * Also the function to eat keyboard events that the application shouldn't receive.
 * Note that this runs in the context of taekwindow.exe.
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
