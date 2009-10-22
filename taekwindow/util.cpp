#include "util.hpp"

void showError(HWND parent, LPCWSTR title, LPCWSTR text, ...) {
	va_list list;
	va_start(list, text);
	PVOID msg;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_STRING, text, 0, 0, (LPWSTR)&msg, 0, &list);
	va_end(list);
	MessageBox(parent, (LPCWSTR)msg, title, MB_OK | MB_ICONERROR);
	LocalFree(msg);
}

void showLastError(HWND parent, LPCWSTR title) {
	PVOID msg;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, (LPWSTR)&msg, 0, NULL);
	MessageBox(parent, (LPCWSTR)msg, title, MB_OK | MB_ICONERROR);
	LocalFree(msg);
}
