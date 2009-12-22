#ifndef MESSAGEWINDOW_HPP
#define MESSAGEWINDOW_HPP

#include <windows.h>

#include "windowproc.hpp"

class MessageWindowClass {

	static TCHAR const *const s_name;

	static size_t s_instances;
	static ATOM s_atom;

	public:

		MessageWindowClass();
		~MessageWindowClass();

		TCHAR const *name() const { return s_name; }

	private:

		void create();
		void destroy();

		static LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

class MessageWindow {

	MessageWindowClass d_class;
	HWND d_handle;
	WindowProcFunc *d_windowProcFunc;

	public:

		/* The functionoid passed in is cloned; it may therefore be a temporary object.
		 */
		MessageWindow();
		~MessageWindow();

		HWND handle() const { return d_handle; }
		void setWindowProc(WindowProcFunc const &proc);

	private:

		MessageWindow(MessageWindow const &); // not implemented
		MessageWindow &operator=(MessageWindow const &); // not implemented

};

#endif
