#ifndef DRAGHANDLER_HPP
#define DRAGHANDLER_HPP

#include "hookhandler.hpp"
#include "dragmachine.hpp"

class DragHandler : public MouseHookHandler {

	DragMachine d_machine;

	public:

		DragHandler();

		virtual bool onMouseDown(MouseDownEvent const &event);
		virtual bool onMouseUp(MouseUpEvent const &event);
		virtual bool onMouseMove(MouseMoveEvent const &event);

};

#endif
