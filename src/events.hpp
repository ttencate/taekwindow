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

#endif