#ifndef UTIL_HPP
#define UTIL_HPP

#include <windows.h>

#pragma warning(disable: 4390) // empty statements

/* Shows a message box with the given message, in which argument substitution is done
 * according to the format of FormatMessage (not printf syntax!).
 * The parent may be NULL.
 */
void showError(HWND parent, LPCTSTR title, LPCTSTR text, ...);

/* Shows a message box with the message corresponding to GetLastError(), with the given title.
 * The parent may be NULL.
 */
void showLastError(HWND parent, LPCTSTR title);

/* Shows a message box with the message corresponding to some system error value.
 */
void showSystemError(HWND parent, DWORD error, LPCTSTR title);

/* Only calls showLastError in debug mode.
 * Useful for checking cases that are very unlikely to go wrong once they go right,
 * but that we want to check during debugging anyway.
 */
#ifdef _DEBUG
#	define debugShowLastError(title) showLastError(NULL, _T(title))
#else
#	define debugShowLastError(title)
#endif

#endif
