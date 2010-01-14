#ifndef DRAGHANDLER_HPP
#define DRAGHANDLER_HPP

#include "hookhandler.hpp"
#include "dragworker.hpp"

class DragHandler : public MouseHookHandler {

	MouseButton d_draggingButton;
	DragWorker *d_worker;

	public:

		DragHandler();

		virtual bool llMouseDown(LLMouseDownEvent const &event);
		virtual bool llMouseUp(LLMouseDownEvent const &event);
		virtual bool llMouseMove(LLMouseMoveEvent const &event);

		virtual bool handleMessage(UINT message, WPARAM wParam, LPARAM lParam);

	private:

		bool isModifierDown() const;

		void handleDragStart(MouseButton button, POINT const &mousePos);
		void handleMoveStart(POINT const &mousePos);
		void handleResizeStart(POINT const &mousePos);
		void handleDragEnd(MouseButton button, POINT const &mousePos);
		void handleDragMove(POINT const &mousePos);

};

#endif
