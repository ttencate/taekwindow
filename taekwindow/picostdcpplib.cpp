#include <windows.h>

void *operator new(size_t size) {
	return HeapAlloc(GetProcessHeap(), 0, size);
}

void operator delete(void *mem) {
	HeapFree(GetProcessHeap(), 0, mem);
}
