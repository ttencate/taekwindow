#ifndef PUSHHANDLER_HPP
#define PUSHHANDLER_HPP

#include "hookhandler.hpp"

class PushHandler : public MouseHookHandler {

	public:

		virtual bool llMouseDown(LLMouseDownEvent const &event);

		virtual bool handleMessage(UINT message, WPARAM wParam, LPARAM lParam);

	private:

		void pushBack(HWND window);
		void forwardMouseDown(HWND window, MouseButton button, POINT const &mousePos);

};

#endif
