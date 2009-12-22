#include <windows.h>
#include <tchar.h>

#include "errors.hpp"
#include "debug.hpp"

HANDLE g_heap;

void initMemMgr() {
#ifdef _DEBUG
	// In debug mode, we allocate a separate heap for our own allocations,
	// because the system uses the default heap and leaks memory all over the place
	// (which is probably normal).
	g_heap = HeapCreate(0, 0, 0);
	if (!g_heap) {
		debugShowLastError("HeapCreate() failed");
	}
#else
	g_heap = GetProcessHeap();
#endif
}

void *operator new(size_t size) {
	void *mem = HeapAlloc(g_heap, 0, size);
	if (!mem) {
		showLastError(NULL, _T("Failed to allocate memory"));
		ExitProcess(1);
	}
	DEBUGLOG("Allocated %d bytes at 0x%08x", size, mem);
	return mem;
}

void operator delete(void *mem) {
	if (!HeapFree(g_heap, 0, mem)) {
		DEBUGLOG("ERROR: Failed to free heap memory at 0x%08x", mem);
		return;
	}
	DEBUGLOG("Freed memory at 0x%08x", mem);
}

#ifdef _DEBUG
void traceLeaks() {
	HANDLE heap = g_heap;

	PROCESS_HEAP_ENTRY entry;
	entry.lpData = NULL;

	HeapLock(heap);
	bool leaks = false;
	while (HeapWalk(heap, &entry)) {
		if (entry.wFlags & PROCESS_HEAP_ENTRY_BUSY) {
			leaks = true;
			DEBUGLOG("WARNING: Leaked block of %d bytes at 0x%08x", entry.cbData, entry.lpData);
		}
	}
	DWORD error = GetLastError();
	HeapUnlock(heap);

	if (error != ERROR_NO_MORE_ITEMS) {
		SetLastError(error);
		showLastError(NULL, _T("HeapWalk() failed"));
	}

	if (leaks) {
		showError(NULL, _T("Memory leaks detected"), _T("Warning: one or more memory leaks were detected at shutdown. See the debug log for details."));
	}
}
#endif
