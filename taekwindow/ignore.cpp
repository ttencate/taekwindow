#include "ignore.hpp"
#include "debuglog.hpp"

IgnoreState::IgnoreState(MouseButton button)
:
	MouseDownState(button)
{
}

void IgnoreState::enter() {
	MouseDownState::enter();

	DEBUGLOG("Starting ignore action");
	cursorWindow().setCursor(crIgnore);
}
