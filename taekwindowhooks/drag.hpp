#ifndef _DRAG_HPP_
#define _DRAG_HPP_

#include <windows.h>

#include "util.hpp"
#include "offset_ptr.hpp"

enum DragState {
	dsNormal, dsMove, dsMaximizedMove, dsResize, dsIgnore
};

/* The dragging system is essentially a state machine. Its input consists of mouse events.
 * Upon state transitions, it may do something. This class is the ancestor of all states.
 */
class BaseState {
public:
	/* Called just before this state is entered. Overriding implementations must call the base function.
	 */
	virtual void enter();

	/* Called just before this state is exited. Overriding implementations must call the base function.
	 */
	virtual void exit()
	{ }

	/* Called when a mouse button is pressed.
	 * window is the bottommost window in the hierarchy. mousePos is the location of the mouse cursor.
	 * If true is returned, the event will not be passed to the application.
	 */
	virtual bool onMouseDown(MouseButton button, HWND window, POINT mousePos) = 0;

	/* Called when a mouse button is released.
	 * window is the bottommost window in the hierarchy. mousePos is the location of the mouse cursor.
	 * If true is returned, the event will not be passed to the application.
	 */
	virtual bool onMouseUp(MouseButton button, HWND window, POINT mousePos) = 0;

	/* Called when the mouse is moved. Overriding functions must call the base function.
	 * If true is returned, the event will not be passed to the application.
	 */
	virtual bool onMouseMove(POINT mousePos) = 0;
};

/* The normal state, when no cool dragging stuff is going on.
 */
class NormalState : public BaseState {
public:
	/* Switches to another state if the button press is of interest to us.
	 */
	virtual bool onMouseDown(MouseButton button, HWND window, POINT mousePos);

	/* Does nothing, not even eat events.
	 */
	virtual bool onMouseUp(MouseButton button, HWND window, POINT mousePos)
	{ return false; }

	/* Does nothing, not even eat events.
	 */
	virtual bool onMouseMove(POINT mousePos)
	{ return false; }
};

/* The base for all states when some button is currently pressed down.
 */
class MouseDownState : public BaseState {
public:
	/* Does nothing, but eats the event.
	 */
	virtual bool onMouseDown(MouseButton button, HWND window, POINT mousePos)
	{ return true; }

	/* Returns us to the normal state if the button that was previously down was released.
	 */
	virtual bool onMouseUp(MouseButton button, HWND window, POINT mousePos);

	/* Stores the button for later use.
	 */
	virtual void enter(MouseButton button);
};

/* The base state for both moving and resizing actions. These have much in common.
 */
class DeformState : public MouseDownState {
public:
	/* Stores the initial state for later use.
	 * Note that parentWindow refers to the window we should move/resize,
	 * not the one bottommost window that receives the actual events.
	 */
	virtual void enter(MouseButton button, HWND parentWindow, POINT mousePos);

	/* Ends the drag action.
	 */
	virtual void exit();
};

/* The state active while the user is moving a window.
 */
class MoveState : public DeformState {
public:
	/* Moves the window accordingly.
	 */
	virtual bool onMouseMove(POINT mousePos);

	/* Sets up the cursor.
	 */
	virtual void enter(MouseButton button, HWND parentWindow, POINT mousePos);

	/* Restores the cursor.
	 */
	virtual void exit();
};

/* The state active while the user is moving a maximized window.
 * This is only possible across monitors; the window will retain its maximized state.
 */
class MaximizedMoveState : public DeformState {
public:
	/* Moves the window accordingly.
	 */
	virtual bool onMouseMove(POINT mousePos);

	/* Sets up the cursor.
	 */
	virtual void enter(MouseButton button, HWND parentWindow, POINT mousePos);

	/* Restores the cursor.
	 */
	virtual void exit();
};

/* The state active while the user is resizing a window.
 */
class ResizeState : public DeformState {
public:
	/* Resizes the window accordingly.
	 */
	virtual bool onMouseMove(POINT mousePos);

	/* Sets up the cursor and the resize type.
	 */
	virtual void enter(MouseButton button, HWND parentWindow, POINT mousePos);

	/* Restores the cursor.
	 */
	virtual void exit();
};

/* The ignore state, activated when the user tries to move/resize a window that is not movable or resizable.
 * This state makes sure that those events are not passed to the application.
 */
class IgnoreState : public MouseDownState {
	/* Does nothing, but eats the event.
	 */
	virtual bool onMouseDown(MouseButton button, HWND window, POINT mousePos)
	{ return true; }

	/* Does nothing, but eats the event.
	 */
	virtual bool onMouseMove(POINT mousePos)
	{ return true; }
};

extern offset_ptr<BaseState> currentState;

#endif
