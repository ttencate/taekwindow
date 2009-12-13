#ifndef PUSHHANDLER_HPP
#define PUSHHANDLER_HPP

#include "hookhandler.hpp"

class PushHandler : public MouseHookHandler {

	public:

		virtual bool onMouseDown(MouseDownEvent const &event);

	private:

		void pushBack(HWND window);

};

#endif
