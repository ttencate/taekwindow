#ifndef _UTIL_HPP_
#define _UTIL_HPP_

#include "types.hpp"

#include <windows.h>

/* For a given mouse event (e.g. WM_LBUTTONDOWN), returns the corresponding button from our enum.
 */
MouseButton eventToButton(WPARAM wParam);

#ifdef DEBUG
/* For debugging purposes: shows a messagebox with the message corresponding to GetLastError(),
 * with the given title.
 */
void showLastError(LPCWSTR title);
#endif

#endif
