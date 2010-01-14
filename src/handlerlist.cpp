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

bool MouseHandlerList::llMouseDown(LLMouseDownEvent const &event) {
	for (size_t i = 0; i < d_count; ++i) {
		if (d_handlers[i]->llMouseDown(event))
			return true;
	}
	return false;
}

bool MouseHandlerList::llMouseUp(LLMouseUpEvent const &event) {
	for (size_t i = 0; i < d_count; ++i) {
		if (d_handlers[i]->llMouseUp(event))
			return true;
	}
	return false;
}

bool MouseHandlerList::llMouseMove(LLMouseMoveEvent const &event) {
	for (size_t i = 0; i < d_count; ++i) {
		if (d_handlers[i]->llMouseMove(event))
			return true;
	}
	return false;
}

bool MouseHandlerList::llMouseWheel(LLMouseWheelEvent const &event) {
	for (size_t i = 0; i < d_count; ++i) {
		if (d_handlers[i]->llMouseWheel(event))
			return true;
	}
	return false;
}

bool MouseHandlerList::handleMessage(UINT message, WPARAM wParam, LPARAM lParam) {
	for (size_t i = 0; i < d_count; ++i) {
		if (d_handlers[i]->handleMessage(message, wParam, lParam))
			return true;
	}
	return false;
}
