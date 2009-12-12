#include <windows.h>
#include <tchar.h>

#include "messages.hpp"
#include "trayicon.hpp"
#include "util.hpp"
#include "config.hpp"
#include "version.h"
#include "hooks.hpp"
#include "debuglog.hpp"
#include "cursors.hpp"

HINSTANCE currentInstance = NULL;

HHOOK lowLevelMouseHook = NULL;
HHOOK lowLevelKeyboardHook = NULL;

Configuration activeConfig;

HINSTANCE getCurrentInstance() {
	return currentInstance;
}

/* Attaches global event hooks.
 * Returns true on success.
 */
bool attachHooks() {
	lowLevelMouseHook = SetWindowsHookEx(WH_MOUSE_LL, lowLevelMouseProc, NULL, NULL);
	if (!lowLevelMouseHook)
		return false;
	lowLevelKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, lowLevelKeyboardProc, NULL, NULL);
	if (!lowLevelKeyboardHook)
		return false;
	return true;
}

/* Detaches previously set hooks.
 * Returns true on success.
 */
bool detachHooks() {
	bool success = true;
	if (!UnhookWindowsHookEx(lowLevelKeyboardHook))
		success = false;
	lowLevelKeyboardHook = NULL;
	if (!UnhookWindowsHookEx(lowLevelMouseHook))
		success = false;
	lowLevelMouseHook = NULL;
	return success;
}

bool isEnabled() {
	return (lowLevelMouseHook && lowLevelKeyboardHook);
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
