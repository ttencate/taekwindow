#ifndef _HOOKS_H_
#define _HOOKS_H_

#include <windows.h>

DWORD __declspec(dllexport) __stdcall init(DWORD threadId);
void __declspec(dllexport) __stdcall readConfig();

LRESULT __declspec(dllexport) CALLBACK mouseProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT __declspec(dllexport) CALLBACK keyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

#endif
