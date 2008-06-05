#include <windows.h>

#include "hooks.hpp"
#include "drag.hpp"
#include "actions.hpp"
#include "config.hpp"
#include "util.hpp"
#include "offset_ptr.hpp"
#include "debuglog.hpp"

/* The current state we're in.
 */
extern offset_ptr<BaseState> currentState;

#ifdef _DEBUG
/* Steal this from main.cpp.
 */
extern DWORD mainThreadId;
#endif

/* Handles a "push window to the background" event.
 */
void handlePushBack(HWND window) {
	window = GetAncestor(window, GA_ROOT);
	doPushBack(window);
}

/* The function for handling mouse events. This is the reason why we have to use a separate DLL;
 * see the SetWindowsHookEx documentation for details.
 */
LRESULT CALLBACK mouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
	bool processed = false; // Set to true if we don't want to pass the event to the application.
	if (nCode >= 0 && nCode == HC_ACTION) { // If nCode < 0, do nothing as per Microsoft's recommendations.
		// Store last known foreground window for focus hack (see doPushBack in actions.cpp).
		HWND lfw = GetForegroundWindow();
		if (lfw) {
			lastForegroundWindow = lfw;
		}
		MOUSEHOOKSTRUCT *eventInfo = (MOUSEHOOKSTRUCT*)lParam;
		MouseButton button;
		switch (wParam) {
			case WM_LBUTTONDOWN:
			case WM_MBUTTONDOWN:
			case WM_RBUTTONDOWN:
			case WM_NCLBUTTONDOWN:
			case WM_NCMBUTTONDOWN:
			case WM_NCRBUTTONDOWN:
				// Find out which button was pressed.
				button = eventToButton(wParam);
				// Are we pushing the window to the back?
				// TODO: refactor this out using some elegant event-handling scheme
				if (button == config.pushBackButton && eventInfo->wHitTestCode == HTCAPTION) {
					handlePushBack(eventInfo->hwnd);
					processed = true;
					break;
				}
				processed = currentState->onMouseDown(button, eventInfo->hwnd, eventInfo->pt);
				break;
			case WM_LBUTTONUP:
			case WM_MBUTTONUP:
			case WM_RBUTTONUP:
			case WM_NCLBUTTONUP:
			case WM_NCMBUTTONUP:
			case WM_NCRBUTTONUP:
				button = eventToButton(wParam);
				// We only want to take action if it's the current dragging button being released.
				processed = currentState->onMouseUp(button, eventInfo->hwnd, eventInfo->pt);
				break;
			case WM_MOUSEMOVE:
			case WM_NCMOUSEMOVE:
				processed = currentState->onMouseMove(eventInfo->pt);
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
	}
	return CallNextHookEx((HHOOK)37, nCode, wParam, lParam); // first argument ignored
}
