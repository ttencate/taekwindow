#include "util.hpp"
#include "config.hpp"
#include "drag.hpp"
#include "offset_ptr.hpp"

/* The shared data segment.
 * These variables are shared across all instances of the DLL. This is necessary because the hook handler
 * runs in the thread of the window that would have received the event, NOT in the thread of the application
 * that hooked up the handler in the first place.
 * IMPORTANT: Everything in this segment must be initialized in order to actually become shared.
 * This syntax only works with the Microsoft build environment...
 * See http://msdn.microsoft.com/en-us/library/h90dkhs0%28VS.80%29.aspx for more information.
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
MaximizedMoveState maximizedMoveState;
ResizeState resizeState;
IgnoreState ignoreState;
offset_ptr<BaseState> currentState = &normalState;

HWND lastForegroundWindow = NULL;

// BEGIN RIPOUT

/* The monitor that the window is currently on.
 */
HMONITOR currentMonitor; // MaximizedMoveState

/* The side(s) on which the window is resized. Both either -1, 0 or 1.
 */
int resizingX, resizingY; // ResizeState

/* The point at which the mouse cursor was last seen.
 */
POINT lastMousePos; // DeformState

/* The window that we're dragging.
 */
HWND draggedWindow; // DeformState

/* The window in the Z-order previous to the draggedWindow.
 * Used to keep the order intact when calling SetWindowPos.
 */
HWND prevInZOrder; // DeformState

/* The current position of the window. Saves calls to GetWindowRect.
 */
RECT lastRect; // DeformState

/* The cursor that was set by the application, before we changed it.
 */
HCURSOR prevCursor; // DeformState

/* The button that was pressed down and caused us to be in this state.
 */
MouseButton downButton; // MouseDownState

// END RIPOUT

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
