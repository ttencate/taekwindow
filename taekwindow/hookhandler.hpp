#ifndef HOOKHANDLER_HPP
#define HOOKHANDLER_HPP

#include <windows.h>

#include "types.hpp"
#include "events.hpp"

/* An interface for classes that want to handle mouse hook events.
 * Return true if hook processing should stop and the event be eaten.
 * The default implementations do nothing and return false.
 */
class MouseHookHandler {
	
	public:

		virtual bool onMouseDown(MouseDownEvent const &event);
		virtual bool onMouseUp(MouseUpEvent const &event);
		virtual bool onMouseMove(MouseMoveEvent const &event);
		virtual bool onMouseWheel(MouseWheelEvent const &event);

};

#endif
