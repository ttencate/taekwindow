#ifndef _TYPES_HPP_
#define _TYPES_HPP_

/* rmBottomRight:   always resize from the bottom right corner of the window, regardless of where it was grabbed.
 * rmClosestCorner: divide window into 9 equally sized rectangles and pick the appropriate corner/edge when resizing.
 */
enum ResizeMode { rmBottomRight = 0, rmNineRectangles = 1 };

/* dsNone:     nothing special going on, all events simply passed on.
 * dsMoving:   we're moving the window identified by draggedWindow.
 * dsResizing: we're resizing the window identified by draggedWindow.
 * dsIgnoring: we're ignoring and discarding mouse events until the button is released.
 *             This is used when the user attempts to Modifier+drag a window that cannot be moved,
 *             e.g. a maximized window. In that case we don't want to pass those mouse events to that window.
 */
enum DragState { dsNone, dsMoving, dsResizing, dsIgnoring };

/* Represents a button on the mouse.
 */
enum MouseButton { mbLeft = 0, mbMiddle = 1, mbRight = 2, mbNone = 0x7FFF };

#endif