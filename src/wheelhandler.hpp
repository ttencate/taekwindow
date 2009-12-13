#ifndef WHEELHANDLER_HPP
#define WHEELHANDLER_HPP

#include "hookhandler.hpp"

class WheelHandler : public MouseHookHandler {
	
	public:
	
		virtual bool onMouseWheel(MouseWheelEvent const &event);

	private:

		/* Handle a mouse wheel event by dispatching it to the window under the cursor.
		 */
		void forwardMouseWheel(MouseWheelEvent const &event);

};

#endif