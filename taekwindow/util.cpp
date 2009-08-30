#include "util.hpp"

void showError(LPCWSTR title, LPCWSTR text, ...) {
	va_list list;
	va_start(list, text);
	PVOID msg;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_STRING, text, 0, 0, (LPWSTR)&msg, 0, &list);
	va_end(list);
	MessageBox(NULL, (LPCWSTR)msg, title, MB_OK | MB_ICONERROR);
	LocalFree(msg);
}

void showLastError(LPCWSTR title) {
	PVOID msg;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, (LPWSTR)&msg, 0, NULL);
	MessageBox(NULL, (LPCWSTR)msg, title, MB_OK | MB_ICONERROR);
	LocalFree(msg);
}
