#ifndef EVENTS_HPP
#define EVENTS_HPP

#include <windows.h>

#include "types.hpp"

struct LLMouseDownEvent {
	POINT mousePos;
	MouseButton button;
	LLMouseDownEvent(POINT p, MouseButton b) :
		mousePos(p), button(b)
	{ }
};

struct LLMouseUpEvent {
	POINT mousePos;
	MouseButton button;
	LLMouseUpEvent(POINT p, MouseButton b) :
		mousePos(p), button(b)
	{ }
};

struct LLMouseMoveEvent {
	POINT mousePos;
	LLMouseMoveEvent(POINT p) :
		mousePos(p)
	{ }
};

struct LLMouseWheelEvent {
	UINT msg;
	POINT mousePos;
	WORD delta;
	LLMouseWheelEvent(UINT m, POINT p, WORD d) :
		msg(m), mousePos(p), delta(d)
	{ }
};

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
	UINT msg;
	POINT mousePos;
	DWORD mouseData;
	HWND window;
	MouseWheelEvent(UINT m, POINT p, DWORD d, HWND w) :
		msg(m), mousePos(p), mouseData(d), window(w)
	{ }
};

#endif