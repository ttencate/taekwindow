#include "globals.hpp"

Globals *globals;

Globals::Globals()
:
	d_llMouseHook(NULL),
	d_llKeyboardHook(NULL)
{
	d_mouseHandlerList.add(&d_wheelHandler);
	d_mouseHandlerList.add(&d_pushHandler);
	d_mouseHandlerList.add(&d_dragHandler);
}

Globals::~Globals() {
}
