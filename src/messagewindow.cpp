#include <tchar.h>

#include "messagewindow.hpp"
#include "errors.hpp"
#include "version.hpp"

TCHAR const *MessageWindowClass::s_name = _T("MessageWindowClass");

size_t MessageWindowClass::s_instances = 0;
ATOM MessageWindowClass::s_atom = NULL;

MessageWindowClass::MessageWindowClass() {
	if (s_instances == 0)
		create();
	++s_instances;
}

MessageWindowClass::~MessageWindowClass() {
	--s_instances;
	if (s_instances == 0)
		destroy();
}

void MessageWindowClass::create() {
	WNDCLASSEX wndClass;
	wndClass.cbSize = sizeof(wndClass);
	wndClass.style = 0;
	wndClass.lpfnWndProc = &windowProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = sizeof(WindowProcFunc);
	wndClass.hInstance = GetModuleHandle(NULL);
	wndClass.hIcon = NULL;
	wndClass.hCursor = NULL;
	wndClass.hbrBackground = NULL;
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = s_name;
	wndClass.hIconSm = NULL;

	s_atom = RegisterClassEx(&wndClass);
	if (!s_atom) {
		showLastError(NULL, _T("Error registering window class"));
		return;
	}
}

void MessageWindowClass::destroy() {
	UnregisterClass(MAKEINTATOM(s_atom), GetModuleHandle(NULL));
}

LRESULT CALLBACK MessageWindowClass::windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	WindowProcFunc *func = (WindowProcFunc*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	// The pointer might not yet be set; but it is initialized to zero.
	if (func) {
		return func->windowProc(hwnd, msg, wParam, lParam);
	} else {
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

MessageWindow::MessageWindow()
:
	d_handle(NULL),
	d_windowProcFunc(NULL)
{
	HINSTANCE instance = GetModuleHandle(NULL);

	// Create the dummy window that receives the icon's messages.
	d_handle = CreateWindow(d_class.name(), _T(APPLICATION_TITLE), 0, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, HWND_MESSAGE, NULL, instance, 0);
	if (!d_handle) {
		showLastError(NULL, _T("Error creating window"));
		return;
	}
}

MessageWindow::~MessageWindow() {
	if (d_handle)
		DestroyWindow(d_handle);
	delete d_windowProcFunc;
}

void MessageWindow::setWindowProc(WindowProcFunc const &proc) {
	if (!d_handle)
		return;

	delete d_windowProcFunc;
	d_windowProcFunc = proc.clone();

	SetLastError(0);
	SetWindowLongPtr(d_handle, GWLP_USERDATA, (LONG_PTR)d_windowProcFunc);
	if (GetLastError()) {
		showLastError(NULL, _T("Failed to set window user data"));
		return;
	}
}