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

protected:
	/* The button that was pressed down and caused us to be in this state.
	 */
	MouseButton downButton;
};

/* The base state for both moving and resizing actions. These have much in common.
 */
class DeformState : public MouseDownState {
public:
	/* Stores the initial state for later use.
	 */
	virtual void enter(MouseButton button, HWND window, POINT mousePos);

	/* Ends the drag action.
	 */
	virtual void exit();
protected:
	/* The point at which the mouse cursor was last seen.
	 */
	POINT lastMousePos;

	/* The window that we're dragging.
	 */
	HWND draggedWindow;

	/* The window in the Z-order previous to the draggedWindow.
	 * Used to keep the order intact when calling SetWindowPos.
	 */
	HWND prevInZOrder;

	/* The current position of the window. Saves calls to GetWindowRect.
	 */
	RECT lastRect;

	/* The cursor that was set by the application, before we changed it.
	 */
	HCURSOR prevCursor;

	/* Returns the movement of the mouse since the last time.
	 */
	POINT mouseDelta(POINT const &mousePos);
	
	/* Calls SetWindowPos with the appropriate arguments. Extra flags can be passed in.
	 */
	void updateWindowPos(UINT flags);

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
	virtual void enter(MouseButton button, HWND window, POINT mousePos);

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
	virtual void enter(MouseButton button, HWND window, POINT mousePos);

	/* Restores the cursor.
	 */
	virtual void exit();
protected:
	/* The side(s) on which the window is resized. Both either -1, 0 or 1.
	 */
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
