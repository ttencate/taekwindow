#include "sharedmemory.hpp"

SharedMemory::SharedMemory(const TCHAR *name, size_t size)
:
	d_handle(NULL),
	d_owned(true),
	d_mem(0)
{
	d_handle = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, size, name);
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		d_owned = false;
	}
	if (d_handle) {
		d_mem = MapViewOfFile(d_handle, FILE_MAP_WRITE, 0, 0, 0);
	}
}

SharedMemory::~SharedMemory() {
	if (d_mem) {
		UnmapViewOfFile(d_mem);
	}
	if (d_handle) {
		CloseHandle(d_handle);
	}
}