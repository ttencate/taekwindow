#include "util.hpp"

MouseButton eventToButton(WPARAM wParam) {
	switch (wParam) {
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
			return mbLeft;
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
			return mbMiddle;
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
			return mbRight;
	}
	return mbLeft; // fallback
}

#ifdef _DEBUG
void showLastError(LPCWSTR title) {
	if (!GetLastError())
		return;
	PVOID msg;
	FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, (LPWSTR)&msg, 0, NULL);
	MessageBoxW(NULL, (LPCWSTR)msg, title, MB_OK | MB_ICONERROR);
	LocalFree(msg);
}
#endif
