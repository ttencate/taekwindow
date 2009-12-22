#include "errors.hpp"

void showError(HWND parent, LPCTSTR title, LPCTSTR text, ...) {
	va_list list;
	va_start(list, text);
	PVOID msg;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_STRING, text, 0, 0, (LPTSTR)&msg, 0, &list);
	va_end(list);
	MessageBox(parent, (LPCTSTR)msg, title, MB_OK | MB_ICONERROR);
	LocalFree(msg);
}

void showLastError(HWND parent, LPCTSTR title) {
	showSystemError(parent, GetLastError(), title);
}

void showSystemError(HWND parent, DWORD error, LPCTSTR title) {
	PVOID msg;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, error, 0, (LPTSTR)&msg, 0, NULL);
	MessageBox(parent, (LPCTSTR)msg, title, MB_OK | MB_ICONERROR);
	LocalFree(msg);
}
