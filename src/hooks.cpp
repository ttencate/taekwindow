#include <windows.h>
#include <tchar.h>

#include "hooks.hpp"
#include "globals.hpp"
#include "util.hpp"
#include "debuglog.hpp"
#include "events.hpp"
#include "handlerlist.hpp"

/* Attaches global event hooks.
 * Returns true on success.
 */
bool attachHooks() {
	globals->llMouseHook() = SetWindowsHookEx(WH_MOUSE_LL, lowLevelMouseProc, globals->currentInstance(), NULL);
	if (!globals->llMouseHook())
		return false;
	globals->llKeyboardHook() = SetWindowsHookEx(WH_KEYBOARD_LL, lowLevelKeyboardProc, globals->currentInstance(), NULL);
	if (!globals->llKeyboardHook())
		return false;
	return true;
}

/* Detaches previously set hooks.
 * Returns true on success.
 */
bool detachHooks() {
	bool success = true;
	if (!UnhookWindowsHookEx(globals->llKeyboardHook()))
		success = false;
	globals->llKeyboardHook() = NULL;
	if (!UnhookWindowsHookEx(globals->llMouseHook()))
		success = false;
	globals->llMouseHook() = NULL;
	return success;
}

bool areHooksAttached() {
	return (globals->llMouseHook() && globals->llKeyboardHook());
}

/* Clips the given point to be inside the cursor clip rectangle.
 */
void clipCursor(POINT &pos) {
	RECT clip;
	GetClipCursor(&clip);
	if (pos.x < clip.left)
		pos.x = clip.left;
	if (pos.x >= clip.right)
		pos.x = clip.right;
	if (pos.y < clip.top)
		pos.y = clip.top;
	if (pos.y >= clip.bottom)
		pos.y = clip.bottom;
}



/* The function for handling mouse events.
 */
LRESULT CALLBACK lowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
	bool eat = false; // Set to true if we don't want to pass the event to the application.
	if (nCode >= 0 && nCode == HC_ACTION) { // If nCode < 0, do nothing as per Microsoft's recommendations.
		MSLLHOOKSTRUCT *eventInfo = (MSLLHOOKSTRUCT*)lParam;
		POINT mousePos = eventInfo->pt;
		// A low-level mouse proc gets the mouse coordinates even before they are
		// clipped to the screen boundaries. So we need to do this ourselves.
		clipCursor(mousePos);
		HWND window = WindowFromPoint(mousePos);
		MouseButton button = eventToButton(wParam);
		switch (wParam) {
			case WM_LBUTTONDOWN:
			case WM_MBUTTONDOWN:
			case WM_RBUTTONDOWN:
			case WM_NCLBUTTONDOWN:
			case WM_NCMBUTTONDOWN:
			case WM_NCRBUTTONDOWN:
				eat = globals->mouseHandlerList().onMouseDown(MouseDownEvent(mousePos, button, window));
				break;
			case WM_LBUTTONUP:
			case WM_MBUTTONUP:
			case WM_RBUTTONUP:
			case WM_NCLBUTTONUP:
			case WM_NCMBUTTONUP:
			case WM_NCRBUTTONUP:
				eat = globals->mouseHandlerList().onMouseUp(MouseUpEvent(mousePos, button, window));
				break;
			case WM_MOUSEMOVE:
			case WM_NCMOUSEMOVE:
				eat = globals->mouseHandlerList().onMouseMove(MouseMoveEvent(mousePos));
				if (eat) {
					// If we eat the event, even the mouse cursor position won't be updated
					// by Windows, so low-level is the low-level hook.
					SetCursorPos(mousePos.x, mousePos.y);
				}
				break;
			case WM_MOUSEWHEEL:
				eat = globals->mouseHandlerList().onMouseWheel(MouseWheelEvent(mousePos, eventInfo->mouseData, window));
				break;
		}
	}

	LRESULT res = CallNextHookEx((HHOOK)37, nCode, wParam, lParam); // first argument ignored
	if (eat)
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
		// DEBUGLOG("vkCode = 0x%08X, flags = 0x%08X", info->vkCode, info->flags);
		if (info->vkCode == 0x51) {
			DEBUGLOG("Panic button pressed");
			// Q button pressed. Panic button for debugging.
			PostThreadMessage(GetCurrentThreadId(), WM_QUIT, 0, 0);
			return 1;
		}
#endif
	}
	return CallNextHookEx((HHOOK)37, nCode, wParam, lParam); // first argument ignored
}
