#ifndef _DRAG_HPP_
#define _DRAG_HPP_

#include <windows.h>

#include "util.hpp"

/* Initiates a window dragging action (i.e. moving/resizing).
 */
void startDragAction(MouseButton button, MOUSEHOOKSTRUCT *eventInfo);

/* Processes drag events that we want to handle.
 */
void doDragAction(MOUSEHOOKSTRUCT const *eventInfo);

/* Processes the end of a drag (i.e. move/resize) action.
 */
void endDragAction();

#endif
