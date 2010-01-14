#include <windowsx.h>

#include "winutils.hpp"

DWORD pointToDword(POINT const &pt) {
	return (((pt).y << 16) | ((pt).x & 0xFFFF));
}

POINT dwordToPoint(DWORD dw) {
	POINT pt = { GET_X_LPARAM(dw), GET_Y_LPARAM(dw) };
	return pt;
}
