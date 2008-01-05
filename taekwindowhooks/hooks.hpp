#ifndef _HOOKS_H_
#define _HOOKS_H_

#include <windows.h>

LRESULT CALLBACK mouseProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK keyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

#endif
