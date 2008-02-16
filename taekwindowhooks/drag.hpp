#ifndef _DRAG_HPP_
#define _DRAG_HPP_

#include <windows.h>

#include "util.hpp"

/* Similar to GetAncestor, but works on MDI windows as well.
 * Returns the first parent in the parent chain of the given window
 * that is movable/resizable (depending on wantResizable),
 * or NULL if there is no such parent.
 */
HWND findGrabbedParent(HWND window, bool wantResizable);

/* Returns true if we should allow moving of this window.
 */
bool isMovableWindow(HWND window);

/* Returns true if we should allow resizing of this window.
 */
bool isResizableWindow(HWND window);

/* Initiates a window moving action.
 */
void startMoveAction(HWND window, POINT mousePos);

/* Initiates a window resizing action.
 */
void startResizeAction(HWND window, POINT mousePos);

/* Processes mouse movements during moving.
 */
void doMoveAction(POINT mousePos);

/* Processes mouse movements during resizing.
 */
void doResizeAction(POINT mousePos);

/* Processes the end of a move action.
 */
void endMoveAction();

/* Processes the end of a resize action.
 */
void endResizeAction();

#endif
