#ifndef KEYBOARDHOOK_HPP
#define KEYBOARDHOOK_HPP

#include "hook.hpp"

class KeyboardHook : public Hook {

	public:

		KeyboardHook();

	private:

		static LRESULT CALLBACK llKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

};

#endif
