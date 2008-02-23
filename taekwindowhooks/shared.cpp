#include "util.hpp"
#include "config.hpp"

#include <stdio.h>

/* The shared data segment.
 * These variables are shared across all instances of the DLL. This is necessary because the hook handler
 * runs in the thread of the window that would have received the event, NOT in the thread of the application
 * that hooked up the handler in the first place.
 * Everything in this segment must be initialized in order to actually become shared.
 * This syntax probably only works with the Microsoft compiler...
 * See http://support.microsoft.com/kb/125677 for more information.
 */
#pragma data_seg(".SHARED")

// Program state

DWORD mainThreadId = 0;
DWORD mainProcessId = 0;

DragState currentState = dsNone;

int resizingX = 0, resizingY = 0;

MouseButton draggingButton = mbNone;

POINT lastMousePos = { 0, 0 };

HWND draggedWindow = NULL;

RECT lastRect = { 0, 0, 0, 0 };

bool modifierDown = false;

bool haveDragged = false;

HCURSOR prevCursor;

HWND lastForegroundWindow = NULL;

// Configuration options

DLLConfiguration config = {0};

// Debugging

#ifdef _DEBUG
/* Handle is valid only in the process that opened the log.
 * Other processes will duplicate it into their own local address space.
 */
HANDLE debugLogFile = INVALID_HANDLE_VALUE;
#endif

/* End of the shared data segment.
 */
#pragma data_seg()
#pragma comment(linker, "/section:.SHARED,rws")
