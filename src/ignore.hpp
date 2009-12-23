#ifndef IGNORE_HPP
#define IGNORE_HPP

#include "mousedown.hpp"

class IgnoreState : public MouseDownState {

	public:

		IgnoreState(MouseButton button);

		virtual void enter();
		virtual void exit();

		virtual bool onMouseDown(MouseDownEvent const &event);
		virtual bool onMouseUp(MouseUpEvent const &event);
		virtual bool onMouseMove(MouseMoveEvent const &event);

};

#endif
