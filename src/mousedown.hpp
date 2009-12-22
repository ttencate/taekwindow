#ifndef MOUSEDOWN_HPP
#define MOUSEDOWN_HPP

#include "dragstate.hpp"
#include "cursorwindow.hpp"

class MouseDownState : public DragState {

	/* The button that was pressed down and caused us to be in this state.
	 */
	MouseButton d_downButton;

	/* The cursor window.
	 */
	CursorWindow *d_cursorWindow;

	public:

		MouseDownState(MouseButton button);

		virtual void enter();
		virtual void exit();

		virtual bool onMouseUp(MouseUpEvent const &event);

	protected:

		CursorWindow &cursorWindow() const { return *d_cursorWindow; }
};

#endif
