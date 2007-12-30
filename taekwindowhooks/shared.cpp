#include "util.hpp"

/* The shared data segment.
 * These variables are shared across all instances of the DLL. This is necessary because the hook handler
 * runs in the thread of the window that would have received the event, NOT in the thread of the application
 * that hooked up the handler in the first place.
 * Everything in this segment must be initialized in order to actually become shared.
 * This syntax probably only works with the Microsoft compiler...
 */
#pragma data_seg(".SHARED")

// Program state

DWORD mainThreadId = 0;

DragState currentState = dsNone;

int resizingX = 0, resizingY = 0;

MouseButton draggingButton = mbLeft;

POINT lastMousePos = { 0, 0 };

HWND draggedWindow = NULL;

RECT lastRect = { 0, 0, 0, 0 };

bool modifierDown = false;

bool haveDragged = false;

// Configuration options

int modifier = VK_MENU;

MouseButton moveButton = mbLeft, resizeButton = mbRight;

ResizeMode resizeMode = rmNineRectangles;

/* End of the shared data segment.
 */
#pragma data_seg()
#pragma comment(linker, "/section:.SHARED,rws")
