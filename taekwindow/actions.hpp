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

/* Activates a window, then restores its original position in the Z order.
 * This has the effect of activating it without bringing it to the front,
 * something that the Windows API normally doesn't allow.
 */
void activateWithoutRaise(HWND window);

#endif
