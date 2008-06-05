#ifndef _DRAG_HPP_
#define _DRAG_HPP_

#include <windows.h>

#include "util.hpp"
#include "offset_ptr.hpp"

// TODO restore comments above everything

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
	 * window is the bottommost window in the hierarchy. pos is the location of the mouse cursor.
	 * If true is returned, the event will not be passed to the application.
	 */
	virtual bool onMouseDown(MouseButton button, HWND window, POINT pos) = 0;

	/* Called when a mouse button is released.
	 * window is the bottommost window in the hierarchy. pos is the location of the mouse cursor.
	 * If true is returned, the event will not be passed to the application.
	 */
	virtual bool onMouseUp(MouseButton button, HWND window, POINT pos) = 0;

	/* Called when the mouse is moved. Overriding functions must call the base function.
	 * If true is returned, the event will not be passed to the application.
	 */
	virtual bool onMouseMove(POINT pos) = 0;
};

/* The normal state, when no cool dragging stuff is going on.
 */
class NormalState : public BaseState {
public:
	/* Switches to another state if the button press is of interest to us.
	 */
	virtual bool onMouseDown(MouseButton button, HWND window, POINT pos);

	/* Does nothing.
	 */
	virtual bool onMouseUp(MouseButton button, HWND window, POINT pos)
	{ return false; }

	/* Does nothing.
	 */
	virtual bool onMouseMove(POINT pos)
	{ return false; }

protected:
	/* Similar to GetAncestor, but works on MDI windows as well.
	 * Returns the first parent in the parent chain of the given window
	 * that is movable/resizable (depending on wantResizable),
	 * or NULL if there is no such parent.
	 */
	static HWND findGrabbedParent(HWND window, bool wantResizable);

	/* Returns true if the modifier key is currently down.
	 */
	static bool isModifierDown();

	/* Returns true if we should allow moving of this window.
	 */
	static bool isMovableWindow(HWND window);

	/* Returns true if we should allow resizing of this window.
	 */
	static bool isResizableWindow(HWND window);

	/* Returns true if the window is fullscreen.
	 */
	static bool isFullscreenWindow(HWND window);
};

/* The base for all states when some button is currently pressed down.
 */
class MouseDownState : public BaseState {
public:
	virtual bool onMouseDown(MouseButton button, HWND window, POINT pos)
	{ return true; }

	virtual bool onMouseUp(MouseButton button, HWND window, POINT pos);

	virtual void enter(MouseButton button);
protected:
	MouseButton downButton;
};

/* The base state for both moving and resizing actions. These have much in common.
 */
class DeformState : public MouseDownState {
public:
	virtual void enter(MouseButton button, HWND window, POINT pos);
	virtual void exit();
protected:
	POINT lastMousePos;
	HWND draggedWindow;
	HWND prevInZOrder;
	RECT lastRect;
	HCURSOR prevCursor;

	/* Sets the new cursor; assumes that the current cursor is defined by the application being dragged.
	 * Expects one of the OCR_* constants.
	 */
	void setCursor(int ocr);

	/* Sets the new cursor; assumes that the current cursor is defined by ourselves.
	 * To be called in between setCursor() and restoreCursor().
	 */
	void updateCursor(int ocr);

	/* Restores the cursor to the one before setCursor() was called.
	 */
	void restoreCursor();

	/* Returns the movement of the mouse since the last time.
	 */
	POINT mouseDelta(POINT const &mousePos);
	
	/* Calls SetWindowPos with the appropriate arguments. Extra flags can be passed in.
	 */
	void updateWindowPos(UINT flags);
};

/* The state active while the user is moving a window.
 */
class MoveState : public DeformState {
public:
	virtual bool onMouseMove(POINT pos);
	virtual void enter(MouseButton button, HWND window, POINT pos);
	virtual void exit();
};

/* The state active while the user is resizing a window.
 */
class ResizeState : public DeformState {
public:
	virtual bool onMouseMove(POINT pos);
	virtual void enter(MouseButton button, HWND window, POINT pos);
	virtual void exit();
protected:
	int resizingX, resizingY;

	/* Sets the variables resizingX and resizingY to the proper values,
	 * considering the client-coordinate pointer location.
	 * Note that, unlike lastRect, these are client coordinates of the dragged window itself,
	 * not those of the dragged window's parent!
	 */
	void setResizingX(POINT const &pt);
	void setResizingY(POINT const &pt);

	/* Returns the cursor (OCR_* constant) to be used for the current resizing direction.
	 */
	int getResizingCursor();
};

/* The ignore state, activated when the user tries to move/resize a window that is not movable or resizable.
 * This state makes sure that those events are not passed to the application.
 */
class IgnoreState : public MouseDownState {
	/* Does nothing.
	 */
	virtual bool onMouseDown(MouseButton button, HWND window, POINT pos)
	{ return true; }

	/* Does nothing.
	 */
	virtual bool onMouseMove(POINT pos)
	{ return true; }
};

extern offset_ptr<BaseState> currentState;

#endif
