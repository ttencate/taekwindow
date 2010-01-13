#include <tchar.h>

#include "mousehook.hpp"
#include "wininfo.hpp"
#include "globals.hpp"
#include "debug.hpp"

MouseHook::MouseHook()
:
	Hook(WH_MOUSE_LL, &llMouseProc)
{
}

/* Clips the given point to be inside the cursor clip rectangle.
 */
void MouseHook::clipCursor(POINT &pos) {
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

bool MouseHook::processMouseMessage(WPARAM wParam, LPARAM lParam) {
	MSLLHOOKSTRUCT *eventInfo = (MSLLHOOKSTRUCT*)lParam;
	POINT mousePos = eventInfo->pt;
	// A low-level mouse proc gets the mouse coordinates even before they are
	// clipped to the screen boundaries. So we need to do this ourselves.
	clipCursor(mousePos);

	UINT message = wParam;
	WPARAM newWParam = eventInfo->mouseData;
	LPARAM newLParam = POINT_TO_LPARAM(mousePos);
	globals->workerThread().postMessage(message, newWParam, newLParam);

	// TODO for the time being, never eat events
	//	case WM_MOUSEMOVE:
				// If we eat the event, even the mouse cursor position won't be updated
				// by Windows, so low-level is the low-level hook.
	//			SetCursorPos(mousePos.x, mousePos.y);
	//			return true;

	return false;
}

/* The function for handling mouse events.
 */
LRESULT CALLBACK MouseHook::llMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
	bool eat = false; // Set to true if we don't want to pass the event to the application.
	if (nCode == HC_ACTION) { // If nCode < 0, do nothing as per Microsoft's recommendations.
		eat = processMouseMessage(wParam, lParam);
	}

	LRESULT res = CallNextHookEx((HHOOK)37, nCode, wParam, lParam); // first argument ignored
	if (eat)
		res = 1; // nonzero return value prevents passing the event to the application
	return res;
}
