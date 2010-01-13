#include "keyboardhook.hpp"
#include "debug.hpp"

KeyboardHook::KeyboardHook()
:
	Hook(WH_KEYBOARD_LL, &llKeyboardProc)
{
}

/* The function for handling keyboard events, tracking the state of the modifier key(s).
 * Also the function to eat keyboard events that the application shouldn't receive.
 * TODO: this is not yet implemented, actually. In the release build this hook is currestly just eating resources...
 * Note that this runs in the context of the main exe.
 */
LRESULT CALLBACK KeyboardHook::llKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	// Note: see the comment in lowLevelMouseProc.
	// If we're going to put more functionality here,
	// it needs to share the "mutex" with that procedure.
	if (nCode == HC_ACTION) {
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
