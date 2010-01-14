#include "ignore.hpp"
#include "debug.hpp"

IgnoreWorker::IgnoreWorker()
:
	DragWorker()
{
	DEBUGLOG("Starting ignore action");
	d_cursorWindow.setCursor(crIgnore);
}

void IgnoreWorker::move(POINT const &) {
	// Ignore it. What else?
}
