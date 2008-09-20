#ifndef _UTIL_HPP_
#define _UTIL_HPP_

#include "../shared/types.hpp"

#include <windows.h>

/* For a given mouse event (e.g. WM_LBUTTONDOWN), returns the corresponding button from our enum.
 */
MouseButton eventToButton(WPARAM wParam);

/* Activates a window, then restores its original position in the Z order.
 * This has the effect of activating it without bringing it to the front,
 * something that the Windows API normally doesn't allow.
 */
void activateWithoutRaise(HWND window);

/* Returns the first parent in the parent chain of the given window
 * that meets the given criterium, or NULL if there is no such parent.
 */
HWND findFirstParent(HWND window, bool (*criterium)(HWND));

/* Returns the last parent in the parent chain of the given window
 * that meets the given criterium, or NULL if there is no such parent.
 */
HWND findLastParent(HWND window, bool (*criterium)(HWND));

/* Returns true if the given window has the given window class name.
 */
bool windowHasClass(HWND window, wchar_t const *className);

/* Returns true if the window is currently maximized.
 */
bool isMaximizedWindow(HWND window);

/* Returns true if the window is fullscreen.
 */
bool isFullscreenWindow(HWND window);

/* Returns true if the window has the WS_CAPTION style.
 */
bool isCaptionWindow(HWND window);

/* Returns true if the window has a thick border (i.e. is resizable).
 */
bool isThickBorderWindow(HWND window);

#ifdef _DEBUG
/* For debugging purposes: shows a messagebox with the message corresponding to GetLastError(),
 * with the given title.
 */
void showLastError(LPCWSTR title);
#endif

#endif
