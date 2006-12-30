#ifndef _HOOKS_H_
#define _HOOKS_H_

#include <windows.h>

extern "C" {

	void __declspec(dllexport) __stdcall init(DWORD threadId);

	LRESULT __declspec(dllexport) __stdcall mouseProc(int nCode, WPARAM wParam, LPARAM lParam);

}

#endif
