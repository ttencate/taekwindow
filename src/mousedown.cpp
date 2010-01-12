#include "mousedown.hpp"
#include "normal.hpp"
#include "debug.hpp"

/* Stores the button for later use, and creates the cursor window.
 */
MouseDownState::MouseDownState(MouseButton button)
:
	d_downButton(button),
	d_cursorWindow(NULL)
{
}

void MouseDownState::enter() {
	DEBUGLOG("Entering MouseDownState");
	d_cursorWindow = new CursorWindow();
}

void MouseDownState::exit() {
	delete d_cursorWindow;
	d_cursorWindow = NULL;
	// Send another Alt event, to deactivate the menu in case it was activated.
	sendAltPress();
	DEBUGLOG("Exited MouseDownState");
}

bool MouseDownState::onMouseUp(MouseUpEvent const &event) {
	DEBUGLOG("Handling button up event");
	// We're in a state in which a special button was down.
	if (event.button == d_downButton) {
		// Go back to normal, but eat the event.
		setNextState(new NormalState());
	}
	// Some other button was released; eat that event too.
	return true;
}

void MouseDownState::sendAltPress() {
	// Figure out which Alt key is down.
	WORD key;
	if (GetAsyncKeyState(VK_LMENU) & 0x8000) {
		key = VK_LMENU;
	} else if (GetAsyncKeyState(VK_RMENU) & 0x8000) {
		key = VK_RMENU;
	} else {
		key = VK_MENU;
	}

	// This is called in a situation where the physical Alt key is actually down,
	// so we send a release event first, then a repress event.
	// The physical release of Alt will match up with our simulated repress.
	// That will hopefully lead to the smallest number of confused apps.
	INPUT inputs[2];
	for (int i = 0; i < 2; ++i) {
		inputs[i].type = INPUT_KEYBOARD;
		inputs[i].ki.wVk = VK_MENU;
		inputs[i].ki.dwFlags = 0;
		inputs[i].ki.time = 0;
		inputs[i].ki.dwExtraInfo = 0;
	}
	inputs[0].ki.dwFlags |= KEYEVENTF_KEYUP;
	SendInput(2, inputs, sizeof(INPUT));
}
