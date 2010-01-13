#include <tchar.h>

#include "mousehook.hpp"
#include "globals.hpp"
#include "debug.hpp"

// We want to use this, but it requires WIN32_WINNT to be 0x600 at least (Windows Vista).
// By setting that, we'd risk accidentally using other Vista features,
// damaging XP support... so we just copy the definition here.
#define WM_MOUSEHWHEEL 0x020E

MouseHook::MouseHook()
:
	Hook(WH_MOUSE_LL, &llMouseProc)
{
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

bool processMouseMessage(WPARAM wParam, LPARAM lParam) {
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
			return globals->mouseHandlerList().onMouseDown(MouseDownEvent(mousePos, button, window));
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
		case WM_NCLBUTTONUP:
		case WM_NCMBUTTONUP:
		case WM_NCRBUTTONUP:
			return globals->mouseHandlerList().onMouseUp(MouseUpEvent(mousePos, button, window));
		case WM_MOUSEMOVE:
		case WM_NCMOUSEMOVE:
			if (globals->mouseHandlerList().onMouseMove(MouseMoveEvent(mousePos))) {
				// If we eat the event, even the mouse cursor position won't be updated
				// by Windows, so low-level is the low-level hook.
				SetCursorPos(mousePos.x, mousePos.y);
				return true;
			} else {
				return false;
			}
		case WM_MOUSEWHEEL:
		case WM_MOUSEHWHEEL:
			return globals->mouseHandlerList().onMouseWheel(MouseWheelEvent(wParam, mousePos, eventInfo->mouseData, window));
		default:
			return false;
	}
}

/* The function for handling mouse events.
 */
LRESULT CALLBACK MouseHook::llMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
	// Question: from where is the callback called?
	// Answer: from pretty much anywhere!
	// The callback is called by sending a message, so any function that causes
	// pending sent messages to be processed can cause callback invocations.
	// At least SetWindowPlacement has been observed to do this, as well as GetWindowRect; maybe GetAncestor too.
	// If a call to SetWindowPlacement can indirectly cause a call to the callback,
	// and if SetWindowPlacement itself was called *from* the callback,
	// all kinds of assumptions in this code break, and things go boom pretty hard.
	// 
	// Workaround: ignore nested callback calls while another call is being processed.
	// 
	// Alternative: post messages to a dummy window and have its window procedure process them sequentially.
	// Drawback: because we defer work until after the callback returns,
	//           we no longer have the power to eat messages from here.
	static bool inMouseProc = false;

	bool eat = false; // Set to true if we don't want to pass the event to the application.
	if (!inMouseProc) {
		inMouseProc = true;
		if (nCode == HC_ACTION) { // If nCode < 0, do nothing as per Microsoft's recommendations.
			eat = processMouseMessage(wParam, lParam);
		}
		inMouseProc = false;
	} else {
		DEBUGLOG("Already in mouse callback; ignoring message 0x%08X", wParam);
	}

	LRESULT res = CallNextHookEx((HHOOK)37, nCode, wParam, lParam); // first argument ignored
	if (eat)
		res = 1; // nonzero return value prevents passing the event to the application
	return res;
}
