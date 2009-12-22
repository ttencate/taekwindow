#include <windows.h>
#include <tchar.h>

#include "main.hpp"
#include "trayicon.hpp"
#include "hooks.hpp"
#include "cursors.hpp"
#include "globals.hpp"
#include "memory.hpp"
#include "errors.hpp"
#include "debug.hpp"
#include "version.hpp"

bool enable() {
	if (isEnabled())
		return true;
	if (!attachHooks())
		return false;
	updateTrayIcon();
	return true;
}

bool disable() {
	if (!isEnabled())
		return true;
	if (!detachHooks())
		return false;
	updateTrayIcon();
	return true;
}

bool isEnabled() {
	return areHooksAttached();
}

void applyConfig(Configuration const &config) {
	globals->config() = config;
	showTrayIcon(config.systemTrayIcon);
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
	int retVal = -1; // value to be returned eventually, after cleaning up etc.

	// Load the configuration from the registry.
	loadAndApplyConfig();
	cursors.load();

	// Attach the event hooks.
	if (!enable()) {
		showLastError(NULL, _T("Error attaching hooks"));
	} else {
		// main message loop
		retVal = messageLoop();
	}

	// Normal exit.
	showTrayIcon(false);
	// Note that calling detachHooks is OK if attachHooks only partly worked.
	detachHooks();

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
