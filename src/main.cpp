#include <windows.h>
#include <tchar.h>

#include "main.hpp"
#include "trayicon.hpp"
#include "cursors.hpp"
#include "globals.hpp"
#include "messages.hpp"
#include "memory.hpp"
#include "sharedmemory.hpp"
#include "errors.hpp"
#include "debug.hpp"
#include "version.h"

TCHAR const *const SHARED_MEM_NAME = _T("Taekwindow_{5C9807CA-E53F-4d1b-8AF7-FCA0FF68EDA0}");

bool enable() {
	if (isEnabled())
		return true;
	if (!globals->mouseHook().attach()) {
		return false;
	}
	if (!globals->keyboardHook().attach()) {
		globals->mouseHook().detach();
		return false;
	}
	globals->trayIcon().update();
	return true;
}

bool disable() {
	if (!isEnabled())
		return true;
	bool ret = true;
	if (!globals->keyboardHook().detach())
		ret = false;
	if (!globals->mouseHook().detach())
		ret = false;
	globals->trayIcon().update();
	return ret;
}

bool isEnabled() {
	return globals->keyboardHook().attached() && globals->mouseHook().attached();
}

void applyConfig(Configuration const &config) {
	globals->config() = config;
	globals->trayIcon().update();
}

void loadAndApplyConfig() {
	Configuration config;
	config.load();
	applyConfig(config);
}

/* Runs the message loop until it is time to quit.
 * Returns the desired exit status of the program.
 */
int messageLoop() {
	BOOL getMsgRetVal;
	MSG msg;
	do {
		getMsgRetVal = GetMessage(&msg, NULL, 0, 0);
		switch (msg.message) {
			case SHOW_CONFIG_MESSAGE:
				globals->configDlg().show();
				continue;
		}
		if (getMsgRetVal == -1) {
			// error in GetMessage... low-level, panic and abort
			break;
		} else {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	} while (getMsgRetVal);
	if (getMsgRetVal) {
		// GetMessage returned -1: error code.
		return -1;
	} else {
		// Normal exit: return the WPARAM from the WM_QUIT.
		return (int)msg.wParam;
	}
}

/* The main function for the application.
 * Called from entryPoint().
 */
int main() {
	// A piece of shared memory that acts as a mutex, so that we have only one running instance.
	// The shared memory contains the thread id of the thread that created it.
	// Note that the shared memory exists for the lifetime of this object, so it must be in main()'s scope.
	SharedMemory shmem(SHARED_MEM_NAME, sizeof(DWORD));
	if (shmem.owned()) {
		DWORD id = GetCurrentThreadId();
		DEBUGLOG("Shared memory is ours; %d is the first instance");
		*(DWORD*)(shmem.mem()) = id;
	} else {
		DWORD id = *(DWORD*)shmem.mem();
		DEBUGLOG("Running instance %d found, posting message and quitting", id);
		PostThreadMessage(id, SHOW_CONFIG_MESSAGE, 0, 0);
		return 0;
	}

	int retVal = -1; // value to be returned eventually, after cleaning up etc.

	// Load the configuration from the registry.
	loadAndApplyConfig();

	// Attach the event hooks.
	if (!enable()) {
		showLastError(NULL, _T("Error attaching hooks"));
	} else {
		// main message loop
		retVal = messageLoop();
	}

	// Normal exit; detach the hooks first.
	disable();

	return retVal;
}

/* The actual entry point to the application.
 * This replaces WinMainCRTStartup from the CRT.
 */
void entryPoint() {
	OPENDEBUGLOG();
	initMemMgr();
	globals = new Globals();
	int mainret = main();
	delete globals;
	TRACE_LEAKS();
	CLOSEDEBUGLOG();
	ExitProcess(mainret);
}
