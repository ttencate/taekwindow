#include <windows.h>
#include <tchar.h>

#include "util.hpp"

void *operator new(size_t size) {
	void *mem = HeapAlloc(GetProcessHeap(), 0, size);
	if (!mem) {
		showLastError(NULL, _T("Failed to allocate heap memory"));
		ExitProcess(1);
	}
	return mem;
}

void operator delete(void *mem) {
	HeapFree(GetProcessHeap(), 0, mem);
}
