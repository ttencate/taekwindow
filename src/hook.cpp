#include "hook.hpp"
#include "debug.hpp"

Hook::Hook(int type, HOOKPROC proc)
:
	d_type(type),
	d_proc(proc),
	d_handle(NULL)
{
}

bool Hook::attach() {
	ASSERT(!attached());
	d_handle = SetWindowsHookEx(d_type, d_proc, GetModuleHandle(NULL), NULL);
	return d_handle != NULL;
}

bool Hook::detach() {
	ASSERT(attached());
	bool success = true;
	if (!UnhookWindowsHookEx(d_handle))
		success = false;
	d_handle = NULL;
	return success;
}
