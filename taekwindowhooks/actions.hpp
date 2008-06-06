#ifndef _ACTIONS_HPP_
#define _ACTIONS_HPP_

#include <windows.h>

/* The last known window that was in the foreground.
 */
extern HWND lastForegroundWindow;

/* Push a window to the background.
 */
bool doPushBack(HWND window);

/* Handle a mouse wheel event by dispatching it to the window under the cursor.
 */
bool doMouseWheel(HWND window, POINT mousePos, WPARAM wParam);

#endif
