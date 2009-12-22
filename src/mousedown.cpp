#include "mousedown.hpp"
#include "dragmachine.hpp"
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