#ifndef HANDLERLIST_HPP
#define HANDLERLIST_HPP

#include "hookhandler.hpp"

class MouseHandlerList {

	static size_t const d_capacity = 4;
	size_t d_count;
	MouseHookHandler *d_handlers[d_capacity];

	public:

		MouseHandlerList();
		~MouseHandlerList();
		void add(MouseHookHandler *handler);

		bool onMouseDown(MouseDownEvent const &event);
		bool onMouseUp(MouseUpEvent const &event);
		bool onMouseMove(MouseMoveEvent const &event);
		bool onMouseWheel(MouseWheelEvent const &event);

};

extern MouseHandlerList *mouseHandlerList;

void createHookHandlers();
void destroyHookHandlers();

#endif
