#ifndef WINUTILS_HPP
#define WINUTILS_HPP

/* Things that should have been in Windows itself but aren't.
 */

#include <windows.h>

/* We want to use this, but it requires WIN32_WINNT to be 0x600 at least (Windows Vista).
 * By setting that, we'd risk accidentally using other Vista features,
 * damaging XP support... so we just copy the definition here.
 */
#define WM_MOUSEHWHEEL 0x020E

/* Converts a POINT to an LPARAM; the inverse of MAKEPOINT.
 */
DWORD pointToDword(POINT const &pt);

/* Like MAKEPOINTS, but creates a POINT instead of a POINTS.
 */
POINT dwordToPoint(DWORD lParam);

#endif