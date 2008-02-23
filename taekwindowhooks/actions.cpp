#include "actions.hpp"
#include "debuglog.hpp"

void doPushBack(HWND window) {
	DEBUGLOG("Pushing window 0x%08x to the back", window);
	SetWindowPos(window, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
}
