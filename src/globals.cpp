#include "globals.hpp"

Globals *globals;

Globals::Globals() {
	d_mouseHandlerList.add(&d_dragHandler);
	d_mouseHandlerList.add(&d_wheelHandler);
	d_mouseHandlerList.add(&d_pushHandler);
}

Globals::~Globals() {
}
