#include "normal.hpp"
#include "globals.hpp"
#include "dragmachine.hpp"
#include "main.hpp"
#include "debug.hpp"
#include "wininfo.hpp"
#include "move.hpp"
#include "movemax.hpp"
#include "resize.hpp"
#include "ignore.hpp"

bool NormalState::onMouseDown(MouseDownEvent const &event) {
	if (!isModifierDown()) {
		// This is not interesting. Discard ASAP.
		return false;
	}
	if (event.button != globals->config().moveButton && event.button != globals->config().resizeButton) {
		// Wrong button. Discard.
		return false;
	}
	// Yippee! A Modifier-drag event just started that we want to process (or ignore).
	DEBUGLOG("Handling a modifier-mousedown event");
	if (event.button == globals->config().moveButton) {
		// We prefer windows that are not maximized over those that are, which makes sense in an MDI environment.
		// This would be what the user expected.
		HWND parentWindow = findFirstParent(event.window, isRestoredMovableWindow);
		if (parentWindow) {
			setNextState(new MoveState(event.mousePos, event.button, parentWindow));
			return true;
		} else {
			// No unmaximized movable window found; look for a maximized one that can be kicked to another monitor.
			// Only top-level windows can be moved to other monitors, I guess.
			// So prefer the outermost (last) parent that is maximized.
			parentWindow = findLastParent(event.window, isMaximizedMovableWindow);
			if (parentWindow) {
				setNextState(new MoveMaxState(event.mousePos, event.button, parentWindow));
				return true;
			}
		}
	} else if (event.button == globals->config().resizeButton) {
		// Try to find a parent window that we can resize without unmaximizing.
		// This one is probably the one that the user meant.
		HWND parentWindow = findFirstParent(event.window, isRestoredResizableWindow);
		if (parentWindow) {
			setNextState(new ResizeState(event.mousePos, event.button, parentWindow));
			return true;
		} else {
			// No unmaximized window; perhaps we have a maximized one?
			// We prefer the outermost, so that a maximized MDI parent is preferred over a maximized MDI child inside it.
			// This makes the most sense, because users often forget that there is an MDI at all when their MDI childs are maximized.
			parentWindow = findLastParent(event.window, isMaximizedResizableWindow);
			if (parentWindow) {
				setNextState(new ResizeState(event.mousePos, event.button, parentWindow));
				return true;
			}
		}
	} else {
		// Nothing of interest: unused button.
		return false;
	}
	// Unsuitable window. Ignore.
	setNextState(new IgnoreState(event.button));
	return false;
}

bool NormalState::isModifierDown() const {
	return (GetAsyncKeyState(globals->config().modifier) & 0x8000) != 0;
}
