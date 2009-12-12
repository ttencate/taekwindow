#ifndef HOOKS_H
#define HOOKS_H

#include <windows.h>

bool attachHooks();
bool detachHooks();
bool areHooksAttached();

LRESULT CALLBACK lowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK lowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

#endif
