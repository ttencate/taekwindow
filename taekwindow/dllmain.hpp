#ifndef _MAIN_HPP_
#define _MAIN_HPP_

#include "dllconfig.hpp"

#include <windows.h>

DWORD init(DWORD threadId, DWORD processId);
void uninit();
void applyDllConfig(DLLConfiguration *newConfig);

#endif
