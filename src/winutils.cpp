#include <windowsx.h>

#include "winutils.hpp"

DWORD pointToDword(POINT const &pt) {
	return (((pt).y << 16) | ((pt).x & 0xFFFF));
}

POINT dwordToPoint(DWORD dw) {
	POINT pt = { GET_X_LPARAM(dw), GET_Y_LPARAM(dw) };
	return pt;
}

DWORD currentVirtualKeys() {
	DWORD ret;
	if (GetAsyncKeyState(VK_CONTROL)  & 0x8000) ret |= MK_CONTROL;
	if (GetAsyncKeyState(VK_LBUTTON)  & 0x8000) ret |= MK_LBUTTON;
	if (GetAsyncKeyState(VK_MBUTTON)  & 0x8000) ret |= MK_MBUTTON;
	if (GetAsyncKeyState(VK_RBUTTON)  & 0x8000) ret |= MK_RBUTTON;
	if (GetAsyncKeyState(VK_SHIFT)    & 0x8000) ret |= MK_SHIFT;
	if (GetAsyncKeyState(VK_XBUTTON1) & 0x8000) ret |= MK_XBUTTON1;
	if (GetAsyncKeyState(VK_XBUTTON2) & 0x8000) ret |= MK_XBUTTON2;
	return ret;
}