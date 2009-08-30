#ifndef _UTIL_HPP_
#define _UTIL_HPP_

#include <windows.h>

/* Shows a message box with the given message, in which argument substitution is done
 * according to the format of FormatMessage (not printf syntax!).
 */
void showError(LPCWSTR title, LPCWSTR text, ...);

/* Shows a message box with the message corresponding to GetLastError(), with the given title.
 */
void showLastError(LPCWSTR title);

/* Only calls showLastError in debug mode.
 * Useful for checking cases that are very unlikely to go wrong once they go right,
 * but that we want to check during debugging anyway.
 */
#ifdef DEBUG
#	define debugShowLastError(title) showLastError(title)
#else
#	define debugShowLastError(title)
#endif

#endif
