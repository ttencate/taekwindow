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

#ifdef _DEBUG
/* For debugging purposes: shows a messagebox with the message corresponding to GetLastError(),
 * with the given title.
 */
void showLastError(LPCWSTR title);
#endif

#endif
