#include <windows.h>
#include <tchar.h>

#include "main.hpp"
#include "messages.hpp"
#include "trayicon.hpp"
#include "util.hpp"
#include "config.hpp"
#include "version.hpp"
#include "hooks.hpp"
#include "debuglog.hpp"
#include "cursors.hpp"
#include "handlerlist.hpp"

HINSTANCE currentInstance = NULL;

Configuration activeConfig;

HINSTANCE getCurrentInstance() {
	return currentInstance;
}

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
	activeConfig = config;
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
int myMain(HINSTANCE hInstance) {
	currentInstance = hInstance;
	int retVal = -1; // value to be returned eventually, after cleaning up etc.

	OPENDEBUGLOG();
	// Load the configuration from the registry.
	createHookHandlers();
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
	destroyHookHandlers();
	CLOSEDEBUGLOG();

	return retVal;
}

/* The actual entry point to the application.
 * This replaces WinMainCRTStartup from the CRT.
 */
int entryPoint() {
	int mainret = 0;
	HINSTANCE hInstance = GetModuleHandle(NULL);
	mainret = myMain(hInstance);
	return mainret;
}
