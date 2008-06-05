#include "util.hpp"
#include "config.hpp"
#include "drag.hpp"
#include "offset_ptr.hpp"

#include <stdio.h>

/* The shared data segment.
 * These variables are shared across all instances of the DLL. This is necessary because the hook handler
 * runs in the thread of the window that would have received the event, NOT in the thread of the application
 * that hooked up the handler in the first place.
 * Everything in this segment must be initialized in order to actually become shared.
 * This syntax only works with the Microsoft build environment...
 * See http://support.microsoft.com/kb/125677 for more information.
 * 
 * VERY IMPORTANT: this segment may be mapped to different addresses in different processes.
 * DO NOT store pointers in the shared data segment, not even if they point to something else in this segment.
 * DO NOT put any data structures in here that depend on pointers.
 * Use an offset_ptr for pointing from the shared segment to the shared segment.
 */
#pragma data_seg(".SHARED")

// Program state

DWORD mainThreadId = 0;
DWORD mainProcessId = 0;

NormalState normalState;
MoveState moveState;
ResizeState resizeState;
IgnoreState ignoreState;
offset_ptr<BaseState> currentState = &normalState;

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
