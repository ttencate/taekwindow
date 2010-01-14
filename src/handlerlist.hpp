#ifndef HANDLERLIST_HPP
#define HANDLERLIST_HPP

#include "hookhandler.hpp"

/* The MouseHandlerList does not own its handlers;
 * destroying them is the caller's responsibility.
 */
class MouseHandlerList {

	static size_t const d_capacity = 4;
	size_t d_count;
	MouseHookHandler *d_handlers[d_capacity];

	public:

		MouseHandlerList();
		void add(MouseHookHandler *handler);

		bool llMouseDown(LLMouseDownEvent const &event);
		bool llMouseUp(LLMouseUpEvent const &event);
		bool llMouseMove(LLMouseMoveEvent const &event);
		bool llMouseWheel(LLMouseWheelEvent const &event);

		bool handleMessage(UINT message, WPARAM wParam, LPARAM lParam);

};

#endif
