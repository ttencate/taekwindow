#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <strsafe.h>

#include "trayicon.hpp"
#include "messages.hpp"
#include "version.hpp"
#include "resource.h"
#include "errors.hpp"
#include "main.hpp"
#include "globals.hpp"

const UINT ICON_ID = 42;

const UINT_PTR IDM_ENABLE    = 1;
const UINT_PTR IDM_CONFIGURE = 2;
const UINT_PTR IDM_EXIT      = 3;

TrayIcon::TrayIcon()
:
	d_showing(false)
{
	HINSTANCE instance = GetModuleHandle(NULL);
	d_enabledIcon = (HICON)LoadImage(instance, MAKEINTRESOURCE(IDI_TRAY_COLOUR), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	d_disabledIcon = (HICON)LoadImage(instance, MAKEINTRESOURCE(IDI_TRAY_GRAY), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE);

	// Create the notify icon itself.
	// We use Version 5.0, that is, Windows 2000/XP semantics.
	d_data.cbSize = sizeof(d_data);
	d_data.hWnd = d_window.handle();
	d_data.uID = ICON_ID;
	d_data.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	d_data.uCallbackMessage = TRAYICON_MESSAGE;
	d_data.hIcon = currentIcon();
	StringCchCopy(d_data.szTip, sizeof(d_data.szTip), _T(APPLICATION_TITLE));
	d_data.dwState = 0;
	d_data.dwStateMask = 0;
	d_data.szInfo[0] = _T('\0');
	d_data.uVersion = NOTIFYICON_VERSION;
	d_data.szInfoTitle[0] = _T('\0');
	d_data.dwInfoFlags = 0;
	if (!Shell_NotifyIcon(NIM_ADD, &d_data)) {
		showLastError(NULL, _T("Error creating notify icon"));
		return;
	}

	d_window.setWindowProc(WindowProcFwd<TrayIcon>(*this, &TrayIcon::windowProc));

	d_showing = true;
}

TrayIcon::~TrayIcon() {
	if (d_showing)
		Shell_NotifyIcon(NIM_DELETE, &d_data);
	if (d_enabledIcon)
		DestroyIcon(d_enabledIcon);
	if (d_disabledIcon)
		DestroyIcon(d_disabledIcon);
}

void TrayIcon::update() {
	if (!d_showing)
		return;
	d_data.hIcon = currentIcon();
	Shell_NotifyIcon(NIM_MODIFY, &d_data);
}

void TrayIcon::showMenu(POINT pos) {
	// Create the pop-up menu for the icon.
	HMENU menu = CreatePopupMenu();
	if (!menu) {
		return;
	}

	// Populate the pop-up menu.
	AppendMenu(menu, (isEnabled() ? MF_CHECKED : 0) | MF_ENABLED | MF_STRING, IDM_ENABLE, _T("En&able Taekwindow"));
	AppendMenu(menu, MF_ENABLED | MF_STRING, IDM_CONFIGURE, _T("&Preferences..."));
	AppendMenu(menu, MF_SEPARATOR, 0, NULL);
	AppendMenu(menu, MF_ENABLED | MF_STRING, IDM_EXIT, _T("&Exit"));

	// Show the pop-up menu.
	// Set the foreground window, so that the menu will be closed when the user clicks elsewhere.
	// Post a dummy message to get it to show up the next time (I don't see this problem myself, but the KB says it).
	// See Microsoft KB article 135788: "PRB: Menus for Notification Icons Do Not Work Correctly".
	SetForegroundWindow(d_window.handle());
	TrackPopupMenuEx(menu,
		TPM_RIGHTBUTTON | (GetSystemMetrics(SM_MENUDROPALIGNMENT) ? TPM_RIGHTALIGN : TPM_LEFTALIGN),
		pos.x, pos.y, d_window.handle(), NULL);
	PostMessage(d_window.handle(), WM_NULL, 0, 0);

	DestroyMenu(menu);
}

HICON TrayIcon::currentIcon() {
	return isEnabled() ? d_enabledIcon : d_disabledIcon;
}

void TrayIcon::toggleEnabled() {
	if (isEnabled()) {
		disable();
	} else {
		enable();
	}
}

void TrayIcon::showConfigDlg() {
	globals->configDlg().show();
}

void TrayIcon::exitProgram() {
	PostQuitMessage(0);
}

LRESULT TrayIcon::windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		case TRAYICON_MESSAGE:
			switch (lParam) { // that contains the "real" message
				case WM_RBUTTONUP:
				case WM_CONTEXTMENU:
				{
					// Using GetMessagePos() would be way better, but it gives me 0.
					POINT mousePos;
					GetCursorPos(&mousePos);
					showMenu(mousePos);
					return 0;
				}
				case WM_LBUTTONDOWN:
				case NIN_SELECT:
				case NIN_KEYSELECT:
					toggleEnabled();
					return 0;
				case WM_LBUTTONDBLCLK:
					toggleEnabled(); // second click does not register as WM_LBUTTONDOWN
					showConfigDlg();
					return 0;
			}
			break;
		case WM_COMMAND:
			switch (LOWORD(wParam)) { // contains the menu item identifier
				case IDM_ENABLE:
					toggleEnabled();
					return 0;
				case IDM_CONFIGURE:
					showConfigDlg();
					return 0;
				case IDM_EXIT:
					exitProgram();
					return 0;
			}
			break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

