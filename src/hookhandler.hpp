#ifndef HOOKHANDLER_HPP
#define HOOKHANDLER_HPP

#include "events.hpp"

/* An interface for classes that want to handle mouse hook events.
 * Return true if hook processing should stop and the event be eaten.
 * The default implementations do nothing and return false.
 */
class MouseHookHandler {
	
	public:

		/* These are called from the mouse hook.
		 * Ideally, they must not call any API functions, because those
		 * might cause the mouse hook callback to be called again.
		 * (See also the comment for the WorkerThread class.)
		 */
		virtual bool llMouseDown(LLMouseDownEvent const &event);
		virtual bool llMouseUp(LLMouseUpEvent const &event);
		virtual bool llMouseMove(LLMouseMoveEvent const &event);
		virtual bool llMouseWheel(LLMouseWheelEvent const &event);

		/* These are called from the worker thread.
		 * Return true if the message was processed.
		 */
		virtual bool handleMessage(UINT message, WPARAM wParam, LPARAM lParam);

};

#endif
