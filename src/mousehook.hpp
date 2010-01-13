#ifndef HOOKS_H
#define HOOKS_H

#include "hook.hpp"

class MouseHook : public Hook {

	public:

		MouseHook();

	private:

		static LRESULT CALLBACK llMouseProc(int nCode, WPARAM wParam, LPARAM lParam);

};

#endif
