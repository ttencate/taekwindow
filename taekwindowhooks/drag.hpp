#ifndef _DRAG_HPP_
#define _DRAG_HPP_

#include <windows.h>

#include "shared.hpp"

/* Initiates a window dragging action (i.e. moving/resizing).
 */
void startDragAction(MouseButton button, MOUSEHOOKSTRUCT *eventInfo);

/* Processes the end of a drag (i.e. move/resize) action.
 */
void endDragAction();

/* Processes drag events that we want to handle.
 */
void processDrag(MOUSEHOOKSTRUCT const *eventInfo);

#endif
