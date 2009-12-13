#include <cstdarg>

#include "handlerlist.hpp"
#include "draghandler.hpp"
#include "wheelhandler.hpp"
#include "pushhandler.hpp"

MouseHandlerList *mouseHandlerList = NULL;

MouseHandlerList::MouseHandlerList()
:
	d_count(0)
{
}

MouseHandlerList::~MouseHandlerList() {
	for (size_t i = 0; i < d_count; ++i) {
		delete d_handlers[i];
	}
}

void MouseHandlerList::add(MouseHookHandler *handler) {
	if (d_count >= d_capacity)
		return;
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

void createHookHandlers() {
	mouseHandlerList = new MouseHandlerList();
	mouseHandlerList->add(new WheelHandler());
	mouseHandlerList->add(new PushHandler());
	mouseHandlerList->add(new DragHandler());
}

void destroyHookHandlers() {
	delete mouseHandlerList;
	mouseHandlerList = NULL;
}

