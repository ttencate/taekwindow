#include "normal.hpp"
#include "dragmachine.hpp"
#include "main.hpp"
#include "debuglog.hpp"
#include "wininfo.hpp"
#include "move.hpp"
#include "movemax.hpp"
#include "resize.hpp"
#include "ignore.hpp"

bool NormalState::onMouseDown(MouseButton button, HWND window, POINT mousePos) {
	if (!isModifierDown()) {
		// This is not interesting. Discard ASAP.
		return false;
	}
	if (button != activeConfig.moveButton && button != activeConfig.resizeButton) {
		// Wrong button. Discard.
		return false;
	}
	// Yippee! A Modifier-drag event just started that we want to process (or ignore).
	DEBUGLOG("Handling a modifier-mousedown event");
	if (button == activeConfig.moveButton) {
		// We prefer windows that are not maximized over those that are, which makes sense in an MDI environment.
		// This would be what the user expected.
		HWND parentWindow = findFirstParent(window, isRestoredMovableWindow);
		if (parentWindow) {
			DragMachine::instance().setNextState(new MoveState(mousePos, button, parentWindow));
			return true;
		} else {
			// No unmaximized movable window found; look for a maximized one that can be kicked to another monitor.
			// Only top-level windows can be moved to other monitors, I guess.
			// So prefer the outermost (last) parent that is maximized.
			parentWindow = findLastParent(window, isMaximizedMovableWindow);
			if (parentWindow) {
				DragMachine::instance().setNextState(new MoveMaxState(mousePos, button, parentWindow));
				return true;
			}
		}
	} else if (button == activeConfig.resizeButton) {
		// Try to find a parent window that we can resize without unmaximizing.
		// This one is probably the one that the user meant.
		HWND parentWindow = findFirstParent(window, isRestoredResizableWindow);
		if (parentWindow) {
			DragMachine::instance().setNextState(new ResizeState(mousePos, button, parentWindow));
			return true;
		} else {
			// No unmaximized window; perhaps we have a maximized one?
			// We prefer the outermost, so that a maximized MDI parent is preferred over a maximized MDI child inside it.
			// This makes the most sense, because users often forget that there is an MDI at all when their MDI childs are maximized.
			parentWindow = findLastParent(window, isMaximizedResizableWindow);
			if (parentWindow) {
				DragMachine::instance().setNextState(new ResizeState(mousePos, button, parentWindow));
				return true;
			}
		}
	} else {
		// Nothing of interest: unused button.
		return false;
	}
	// Unsuitable window. Ignore.
	DragMachine::instance().setNextState(new IgnoreState(button));
	return false;
}

bool NormalState::isModifierDown() const {
	return (GetAsyncKeyState(activeConfig.modifier) & 0x8000) != 0;
}
