#ifndef TRAYICON_HPP
#define TRAYICON_HPP

#include <windows.h>

#include "messagewindow.hpp"

class TrayIcon {

	HICON d_enabledIcon, d_disabledIcon;
	bool d_showing;
	NOTIFYICONDATA d_data;
	MessageWindow d_window;

	public:

		TrayIcon();
		~TrayIcon();

		bool showing() const { return d_showing; }
		void update();

	private:

		void create();
		void destroy();

		HICON currentIcon();

		void showMenu(POINT pos);
		void toggleEnabled();
		void showConfigDlg();
		void exitProgram();

		LRESULT windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		TrayIcon(TrayIcon const &); // not implemented
		TrayIcon &operator=(TrayIcon const &); // not implemented

};

#endif
