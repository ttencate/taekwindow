#include "trayicon.hpp"
#include "messages.hpp"
#include "version.h"
#include "resource.h"
#include "util.hpp"
#include "main.hpp"

#include <windows.h>
#include <windowsx.h>

const UINT ICON_ID = 42;

const UINT_PTR IDM_ENABLE    = 1;
const UINT_PTR IDM_CONFIGURE = 2;
const UINT_PTR IDM_EXIT      = 3;

HINSTANCE instanceHandle;
HICON iconHandle;
ATOM iconWindowClass;
HWND iconWindowHandle;
NOTIFYICONDATA iconData;

bool haveIcon = false;

void doIconMenu(POINT pos) {
	// Create the pop-up menu for the icon.
	HMENU menuHandle = CreatePopupMenu();
	if (!menuHandle) {
		return;
	}

	// Populate the pop-up menu.
	AppendMenu(menuHandle, (isEnabled() ? MF_CHECKED : 0) | MF_ENABLED | MF_STRING, IDM_ENABLE, L"En&able");
	AppendMenu(menuHandle, MF_ENABLED | MF_STRING, IDM_CONFIGURE, L"&Configure...");
	AppendMenu(menuHandle, MF_SEPARATOR, 0, NULL);
	AppendMenu(menuHandle, MF_ENABLED | MF_STRING, IDM_EXIT, L"&Exit");

	// Show the pop-up menu.
	// Set the foreground window, so that the menu will be closed when the user clicks elsewhere.
	// Post a dummy message to get it to show up the next time (I don't this problem myself, but the KB says it).
	// See Microsoft KB article 135788: "PRB: Menus for Notification Icons Do Not Work Correctly".
	SetForegroundWindow(iconWindowHandle);
	TrackPopupMenuEx(menuHandle,
		TPM_RIGHTBUTTON | (GetSystemMetrics(SM_MENUDROPALIGNMENT) ? TPM_RIGHTALIGN : TPM_LEFTALIGN),
		pos.x, pos.y, iconWindowHandle, NULL);
	PostMessage(iconWindowHandle, WM_NULL, 0, 0);

	DestroyMenu(menuHandle);
}

bool loadIcon() {
	WORD iconId = isEnabled() ? TRAYICON_COLOUR : TRAYICON_GRAY;
	HICON newHandle = (HICON)LoadImage(instanceHandle, MAKEINTRESOURCE(iconId), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	if (newHandle) {
		if (iconHandle) {
			DestroyIcon(iconHandle);
		}
		iconHandle = newHandle;
		return true;
	} else {
		return false;
	}
}

void toggleEnabled() {
	if (isEnabled()) {
		disable();
	} else {
		enable();
	}
}

void startConfigurationTool() {
	// Find the full filename of the currently running executable.
	TCHAR path[MAX_PATH+1];
	int length = GetModuleFileName(NULL, path, sizeof(path));
	if (length == 0 || length == sizeof(path)) {
		MessageBox(NULL, L"The path to taekwindow.exe is too long.", L"Could not fetch path to taekwindow.exe", MB_ICONERROR | MB_OK);
		return;
	}

	// Strip off the .exe file name to find the path.
	while (length >= 0 && path[length-1] != '\\') {
		--length;
	}
	if (length < 0) {
		TCHAR buffer[MAX_PATH + 256];
		wsprintf(buffer, L"The path to taekwindow.exe could not be determined, because the full file name does not contain a backslash:\n\n'%s'", path);
		MessageBox(NULL, buffer, L"Could not determine path to taekwindow.exe", MB_ICONERROR | MB_OK);
		return;
	}
	path[length] = L'\0';

	// Launch the configuration tool.
	HINSTANCE res = ShellExecute(NULL, L"open", L"taekwindowconfig.exe", L"", path, SW_SHOWDEFAULT);
	if (res < (HINSTANCE)32) {
		if (res == (HINSTANCE)ERROR_FILE_NOT_FOUND) {
			MessageBox(NULL, L"The file taekwindowconfig.exe was not found in the same directory as taekwindow.exe. Reinstall the application to repair this.", L"Configuration tool cannot be started", MB_ICONERROR | MB_OK);
		} else {
			showLastError(L"Configuration tool cannot be started");
		}
	}
}

void exitProgram() {
	PostQuitMessage(0);
}

LRESULT CALLBACK iconWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case TRAYICON_MESSAGE:
			switch (lParam) { // that contains the "real" message
				case WM_RBUTTONUP:
				case WM_CONTEXTMENU:
					{
						// Using GetMessagePos() would be way better, but it gives me 0.
						POINT mousePos;
						GetCursorPos(&mousePos);
						doIconMenu(mousePos);
						return 0;
					}
				case WM_LBUTTONDOWN:
				case NIN_SELECT:
				case NIN_KEYSELECT:
					toggleEnabled();
					return 0;
				case WM_LBUTTONDBLCLK:
					toggleEnabled(); // second click does not register as WM_LBUTTONDOWN
					startConfigurationTool();
					return 0;
			}
			break;
		case WM_COMMAND:
			switch (LOWORD(wParam)) { // contains the menu item identifier
				case IDM_ENABLE:
					toggleEnabled();
					return 0;
				case IDM_CONFIGURE:
					startConfigurationTool();
					return 0;
				case IDM_EXIT:
					exitProgram();
					return 0;
			}
			break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void createTrayIcon() {
	instanceHandle = GetModuleHandle(NULL);

	// Load the icon.
	loadIcon();
	if (!iconHandle) {
		showLastError(L"Error loading icon");
		return;
	}

	// Create a window class for the dummy window.
	WNDCLASSEX wndClass;
	wndClass.cbSize = sizeof(wndClass);
	wndClass.style = 0;
	wndClass.lpfnWndProc = &iconWindowProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = instanceHandle;
	wndClass.hIcon = NULL;
	wndClass.hCursor = NULL;
	wndClass.hbrBackground = NULL;
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = L"TaekwindowIconWindowClass";
	wndClass.hIconSm = NULL;
	iconWindowClass = RegisterClassEx(&wndClass);
	if (!iconWindowClass) {
		showLastError(L"Error registering window class");
		DestroyIcon(iconHandle);
		return;
	}

	// Create the dummy window that receives the icon's messages.
	iconWindowHandle = CreateWindow(wndClass.lpszClassName, APPLICATION_TITLE_W, 0, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, HWND_MESSAGE, NULL, instanceHandle, 0);
	if (!iconWindowHandle) {
		showLastError(L"Error creating window");
		UnregisterClass((LPCWSTR)iconWindowClass, instanceHandle);
		DestroyIcon(iconHandle);
		return;
	}

	// Finally, create the notify icon itself.
	// We use Version 5.0, that is, Windows 2000/XP semantics.
	iconData.cbSize = sizeof(iconData);
	iconData.hWnd = iconWindowHandle;
	iconData.uID = ICON_ID;
	iconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	iconData.uCallbackMessage = TRAYICON_MESSAGE;
	iconData.hIcon = iconHandle;
	wcscpy_s(iconData.szTip, APPLICATION_TITLE_W);
	iconData.dwState = 0;
	iconData.dwStateMask = 0;
	iconData.szInfo[0] = L'\0';
	iconData.uVersion = NOTIFYICON_VERSION;
	iconData.szInfoTitle[0] = L'\0';
	iconData.dwInfoFlags = 0;
	if (!Shell_NotifyIcon(NIM_ADD, &iconData)) {
		showLastError(L"Error creating notify icon");
		DestroyWindow(iconWindowHandle);
		UnregisterClass((LPCWSTR)iconWindowClass, instanceHandle);
		DestroyIcon(iconHandle);
		return;
	}

	haveIcon = true;
}

void updateTrayIcon() {
	if (!haveIcon)
		return;

	if (loadIcon()) {
		iconData.hIcon = iconHandle;
		if (iconHandle) {
			Shell_NotifyIcon(NIM_MODIFY, &iconData);
			return;
		}
	}
}

void destroyTrayIcon() {
	Shell_NotifyIcon(NIM_DELETE, &iconData);
	DestroyWindow(iconWindowHandle);
	UnregisterClass((LPCWSTR)iconWindowClass, instanceHandle);
	DestroyIcon(iconHandle);
	haveIcon = false;
}

void showTrayIcon(bool show) {
	if (show == haveIcon)
		return;
	if (show)
		createTrayIcon();
	else
		destroyTrayIcon();
}

bool showingTrayIcon() {
	return haveIcon;
}
