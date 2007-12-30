#ifndef _SHARED_HPP_
#define _SHARED_HPP_

#include <windows.h>

/* The thread ID of the first thread that called init().
 */
extern DWORD mainThreadId;

/* The current state we're in.
 * dsNone: nothing special going on, all events simply passed on.
 * dsDragging: we're moving or resizing the window identified by draggedWindow.
 * dsIgnoring: we're ignoring and discarding mouse events until the button is released.
 *             This is used when the user attempts to Modifier+drag a window that cannot be moved,
 *             e.g. a maximized window. In that case we don't want to pass those mouse events to that window.
 */
enum DragState { dsNone, dsDragging, dsIgnoring };
extern DragState currentState;

/* Whether we're resizing in the x and/or y direction. Only meaningful while dragging.
 * resizingX == -1 means resizing at left border, 0 means not resizing in x direction, 1 means right border.
 * Similar for y; -1 is top border, 0 is no resizing, 1 is bottom border.
 */
extern int resizingX, resizingY;

/* The button that we're dragging with.
 * Only meaningful while we're dragging, of course.
 */
enum MouseButton { mbLeft, mbMiddle, mbRight };
extern MouseButton draggingButton;

/* The last known location of the mouse cursor (screen coordinates).
 * This is used in the mouse event handler to compute the distance travelled since the last mouse event.
 */
extern POINT lastMousePos;

/* The window that we are currently dragging.
 * Only meaningful if currentState is dsDragging.
 */
extern HWND draggedWindow;

/* The last known window rectangle of the draggedWindow. Saves us calls to GetWindowRect().
 */
extern RECT lastRect;

/* Whether or not the modifier key is currently down.
 */
extern bool modifierDown;

/* Whether or not dragging has occurred since the last key-down event of the Modifier.
 * If dragging has occurred, this prevents the key-up event to be passed on.
 */
extern bool haveDragged;

#endif
