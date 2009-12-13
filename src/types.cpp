#include "types.hpp"

MouseButton eventToButton(WPARAM wParam) {
	switch (wParam) {
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_NCLBUTTONDOWN:
		case WM_NCLBUTTONUP:
			return mbLeft;
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_NCMBUTTONDOWN:
		case WM_NCMBUTTONUP:
			return mbMiddle;
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_NCRBUTTONDOWN:
		case WM_NCRBUTTONUP:
			return mbRight;
	}
	return mbLeft; // fallback
}
