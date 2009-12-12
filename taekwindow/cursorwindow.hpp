#ifndef CURSORWINDOW_HPP
#define CURSORWINDOW_HPP

#include <windows.h>

#include "cursors.hpp"

/* A fullscreen, topmost, invisible window, so that we can set the cursor.
 * Only one instance of this class may be created at the same time.
 */
class CursorWindow {

	HINSTANCE d_instance;
	ATOM d_class;
	HWND d_handle;

	public:

		CursorWindow();
		~CursorWindow();
		void setCursor(Cursor cursor);

	private:

		ATOM createClass();
		HWND createWindow();
		static LRESULT CALLBACK winProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
#ifdef _DEBUG
		static void paint(HWND window);
#endif
};

#endif
