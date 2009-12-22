#ifndef SHAREDMEMORY_HPP
#define SHAREDMEMORY_HPP

#include <windows.h>

class SharedMemory {

	HANDLE d_handle;
	bool d_owned;
	void *d_mem;

	public:

		SharedMemory(TCHAR const *name, size_t size);
		~SharedMemory();

		bool owned() const { return d_owned; }
		void *mem() const { return d_mem; }
};

#endif
