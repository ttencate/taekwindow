#ifndef HOOKS_H
#define HOOKS_H

#include "hook.hpp"

class MouseHook : public Hook {

	public:

		MouseHook();

	private:

		static LRESULT CALLBACK llMouseProc(int nCode, WPARAM wParam, LPARAM lParam);

		static POINT clipCursor(POINT const &pos);
		static bool processMouseMessage(UINT message, WPARAM mouseData, POINT const &mousePos);
		static bool sendMessageToHandler(UINT message, WPARAM mouseData, POINT const &mousePos);

};

#endif
