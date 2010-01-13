#ifndef HOOK_HPP
#define HOOK_HPP

#include <windows.h>

class Hook {

	int d_type;
	HOOKPROC d_proc;
	HHOOK d_handle;

	public:

		Hook(int type, HOOKPROC proc);

		/* Attaches the hook, returning true upon success.
		 * Assumes it is not attached.
		 */
		bool attach();

		/* Detaches the hook, returning true upon success.
		 * Assumes it is attached.
		 * Regardless of failure, the hook is always considered detached after this call.
		 */
		bool detach();

		/* Returns true if the hook is attached.
		 */
		bool attached() { return d_handle != NULL; }

};

#endif
