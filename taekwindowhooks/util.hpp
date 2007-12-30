#ifndef _UTIL_HPP_
#define _UTIL_HPP_

#include <windows.h>

#ifdef DEBUG
/* For debugging purposes: shows a messagebox with the message corresponding to GetLastError(),
 * with the given title.
 */
void showLastError(LPCWSTR title);
#endif

#endif
