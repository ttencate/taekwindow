#include <windows.h>
#include <tchar.h>

#include "hooks.hpp"
#include "drag.hpp"
#include "actions.hpp"
#include "config.hpp"
#include "util.hpp"
#include "debuglog.hpp"

/* The current state we're in.
 */
extern DragState currentState;

#ifdef _DEBUG
/* Steal this from main.cpp.
 */
extern DWORD mainThreadId;
#endif

/* Handles a possible "push window to the background" event.
 */
bool considerPushBack(MouseButton button, HWND window, POINT mousePos) {
	if (button == config.pushBackButton) {
		UINT hitTestCode = SendMessage(window, WM_NCHITTEST, 0, MAKELPARAM(mousePos.x, mousePos.y));
		if (hitTestCode == HTCAPTION) {
			window = GetAncestor(window, GA_ROOT);
			return doPushBack(window);
		}
	}
	return false;
}

/* Handles a possible scroll wheel event.
 */
bool considerMouseWheel(HWND window, POINT mousePos, WPARAM wParam) {
	if (config.scrollWindowUnderCursor) {
		return doMouseWheel(window, mousePos, wParam);
	} else {
		// Use Windows default behaviour.
		return false;
	}
}

/* The function for handling mouse events. This is the reason why we have to use a separate DLL;
 * see the SetWindowsHookEx documentation for details.
 */
LRESULT CALLBACK lowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
	bool processed = false; // Set to true if we don't want to pass the event to the application.
	if (nCode >= 0 && nCode == HC_ACTION) { // If nCode < 0, do nothing as per Microsoft's recommendations.
		// Store last known foreground window for focus hack (see doPushBack in actions.cpp).
		HWND lfw = GetForegroundWindow();
		if (lfw) {
			lastForegroundWindow = lfw;
		}
		MSLLHOOKSTRUCT *eventInfo = (MSLLHOOKSTRUCT*)lParam;
		HWND window = WindowFromPoint(eventInfo->pt);
		MouseButton button = eventToButton(wParam);
		switch (wParam) {
			case WM_LBUTTONDOWN:
			case WM_MBUTTONDOWN:
			case WM_RBUTTONDOWN:
			case WM_NCLBUTTONDOWN:
			case WM_NCMBUTTONDOWN:
			case WM_NCRBUTTONDOWN:
				// Are we pushing the window to the back?
				processed |= considerPushBack(button, window, eventInfo->pt);
				processed |= onMouseDown(button, window, eventInfo->pt);
				break;
			case WM_LBUTTONUP:
			case WM_MBUTTONUP:
			case WM_RBUTTONUP:
			case WM_NCLBUTTONUP:
			case WM_NCMBUTTONUP:
			case WM_NCRBUTTONUP:
				processed |= onMouseUp(button, window, eventInfo->pt);
				break;
			case WM_MOUSEMOVE:
			case WM_NCMOUSEMOVE:
				processed |= onMouseMove(eventInfo->pt);
				if (processed) {
					// If we eat the event, even the mouse cursor position won't be updated
					// by Windows, so low-level is the low-level hook.
					SetCursorPos(eventInfo->pt.x, eventInfo->pt.y);
				}
				break;
			case WM_MOUSEWHEEL:
				processed |= considerMouseWheel(window, eventInfo->pt, eventInfo->mouseData);
				break;
		}
	}

	LRESULT res = CallNextHookEx((HHOOK)37, nCode, wParam, lParam); // first argument ignored
	if (processed)
		res = 1; // nonzero return value prevents passing the event to the application
	return res;
}

/* The function for handling keyboard events, tracking the state of the modifier key(s).
 * Also the function to eat keyboard events that the application shouldn't receive.
 * TODO: this is not yet implemented, actually. In the release build this hook is currestly just eating resources...
 * Note that this runs in the context of the main exe.
 */
LRESULT CALLBACK lowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode >= 0 && nCode == HC_ACTION) { // A little redundant, yes. But the docs say it.
#ifdef _DEBUG
		KBDLLHOOKSTRUCT *info = (KBDLLHOOKSTRUCT*)lParam;
		DEBUGLOG("vkCode = 0x%08X, flags = 0x%08X", info->vkCode, info->flags);
		if (info->vkCode == 0x51) {
			DEBUGLOG("Panic button pressed");
			// Q button pressed. Panic button for debugging.
			PostThreadMessage(mainThreadId, WM_QUIT, 0, 0);
			return 1;
		}
#endif
	}
	return CallNextHookEx((HHOOK)37, nCode, wParam, lParam); // first argument ignored
}
