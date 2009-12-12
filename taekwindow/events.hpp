#ifndef EVENTS_HPP
#define EVENTS_HPP

#include <windows.h>

#include "types.hpp"

struct MouseDownEvent {
	POINT mousePos;
	MouseButton button;
	HWND window;
	MouseDownEvent(POINT p, MouseButton b, HWND w) :
		mousePos(p), button(b), window(w)
	{ }
};

struct MouseUpEvent {
	POINT mousePos;
	MouseButton button;
	HWND window;
	MouseUpEvent(POINT p, MouseButton b, HWND w) :
		mousePos(p), button(b), window(w)
	{ }
};

struct MouseMoveEvent {
	POINT mousePos;
	MouseMoveEvent(POINT p) :
		mousePos(p)
	{ }
};

struct MouseWheelEvent {
	POINT mousePos;
	DWORD mouseData;
	HWND window;
	MouseWheelEvent(POINT p, DWORD d, HWND w) :
		mousePos(p), mouseData(d), window(w)
	{ }
};

#endif