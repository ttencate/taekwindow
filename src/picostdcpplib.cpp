#include <windows.h>
#include <tchar.h>

#include "errors.hpp"
#include "debug.hpp"

void *operator new(size_t size) {
	void *mem = HeapAlloc(GetProcessHeap(), 0, size);
	if (!mem) {
		showLastError(NULL, _T("Failed to allocate heap memory"));
		ExitProcess(1);
	}
	return mem;
}

void operator delete(void *mem) {
	if (!HeapFree(GetProcessHeap(), 0, mem))
		DEBUGLOG("ERROR: Failed to free heap memory!");
}
