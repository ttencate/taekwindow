#include "handlerlist.hpp"
#include "debug.hpp"

MouseHandlerList::MouseHandlerList()
:
	d_count(0)
{
}

void MouseHandlerList::add(MouseHookHandler *handler) {
	ASSERT(d_count < d_capacity);
	d_handlers[d_count] = handler;
	++d_count;
}

bool MouseHandlerList::onMouseDown(MouseDownEvent const &event) {
	for (size_t i = 0; i < d_count; ++i) {
		if (d_handlers[i]->onMouseDown(event))
			return true;
	}
	return false;
}

bool MouseHandlerList::onMouseUp(MouseUpEvent const &event) {
	for (size_t i = 0; i < d_count; ++i) {
		if (d_handlers[i]->onMouseUp(event))
			return true;
	}
	return false;
}

bool MouseHandlerList::onMouseMove(MouseMoveEvent const &event) {
	for (size_t i = 0; i < d_count; ++i) {
		if (d_handlers[i]->onMouseMove(event))
			return true;
	}
	return false;
}

bool MouseHandlerList::onMouseWheel(MouseWheelEvent const &event) {
	for (size_t i = 0; i < d_count; ++i) {
		if (d_handlers[i]->onMouseWheel(event))
			return true;
	}
	return false;
}
