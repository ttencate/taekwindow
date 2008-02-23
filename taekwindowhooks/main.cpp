#include "main.hpp"
#include "config.hpp"
#include "debuglog.hpp"

/* The handle of this instance of the DLL, set in DllMain.
 */
HINSTANCE dllHandle = NULL;

/* The thread and process ID of the first thread that called init().
 */
extern DWORD mainThreadId;
extern DWORD mainProcessId;

/* The entry point for the DLL. Stores the instance handle of the DLL for later use.
 */
BOOL WINAPI DllMain(HANDLE hinstDLL, DWORD dwReason, LPVOID lpvReserved) {
	if (dwReason == DLL_PROCESS_ATTACH || dwReason == DLL_THREAD_ATTACH) {
		dllHandle = (HINSTANCE)hinstDLL;
	} else if (dwReason == DLL_PROCESS_DETACH || dwReason == DLL_THREAD_DETACH) {
		// Nothing to do for the moment.
	}
	return TRUE;
}

/* Initializes the DLL by storing the ID of the thread that will hook up the event hooks.
 * On successful execution, the thread ID is stored and the function returns NULL.
 * If there was already another thread ID (probably another main exe process),
 * the function returns the thread ID of that thread and does nothing else.
 */
DWORD init(DWORD threadId, DWORD processId) {
	if (mainThreadId) {
		return mainThreadId;
	} else {
		mainThreadId = threadId;
		mainProcessId = processId;
		config.setDefaults();
		OPENDEBUGLOG();
		DEBUGLOG("DLL initialized");
		return NULL;
	}
}

/* Uninitializes the DLL by forgetting the thread ID.
 */
void uninit() {
	DEBUGLOG("DLL uninitializing");
	CLOSEDEBUGLOG();
	mainThreadId = 0;
	mainProcessId = 0;
}

void applyConfig(DLLConfiguration *newConfig) {
	DEBUGLOG("Updating configuration");
	config = *newConfig;
}
