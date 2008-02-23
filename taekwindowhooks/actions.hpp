#ifndef _ACTIONS_HPP_
#define _ACTIONS_HPP_

#include <windows.h>

/* The last known window that was in the foreground.
 */
extern HWND lastForegroundWindow;

void doPushBack(HWND window);

#endif
