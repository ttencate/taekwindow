#ifndef WHEELHANDLER_HPP
#define WHEELHANDLER_HPP

#include "hookhandler.hpp"

class WheelHandler : public MouseHookHandler {
	
	public:
	
		virtual bool llMouseWheel(LLMouseWheelEvent const &event);

		virtual bool handleMessage(UINT message, WPARAM wParam, LPARAM lParam);

	private:

		/* Handle a mouse wheel event by dispatching it to the window under the cursor.
		 */
		void forwardMouseWheel(UINT message, WPARAM wParam, LPARAM lParam);

};

#endif