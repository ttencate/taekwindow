#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include "config.hpp"
#include "cursors.hpp"
#include "wheelhandler.hpp"
#include "pushhandler.hpp"
#include "draghandler.hpp"
#include "handlerlist.hpp"
#include "mousehook.hpp"
#include "keyboardhook.hpp"
#include "workerthread.hpp"
#include "trayiconmanager.hpp"
#include "configsheetmanager.hpp"

/* We need this because we're not using the CRT,
 * so constructors and destructors of globally declared objects are never called.
 * Instead of having to call each individually,
 * we wrap all global variables into this object, create an instance
 * in the entry point function, and set a global pointer to this instance.
 *
 * We need globals in the first place because the hook handlers are callback functions,
 * hence they have no access to any state except the global state.
 */
class Globals {

	// Important: the order of declaration is the order of construction!
	Configuration d_config;
	Cursors d_cursors;
	WheelHandler d_wheelHandler;
	PushHandler d_pushHandler;
	DragHandler d_dragHandler;
	MouseHandlerList d_mouseHandlerList;
	WorkerThread d_workerThread;
	MouseHook d_mouseHook;
	KeyboardHook d_keyboardHook;
	TrayIconManager d_trayIcon;
	ConfigSheetManager d_configDlg;

	public:

		Globals();
		~Globals();

		Configuration &config() { return d_config; }
		Cursors &cursors() { return d_cursors; }
		MouseHandlerList &mouseHandlerList() { return d_mouseHandlerList; }
		WorkerThread &workerThread() { return d_workerThread; }
		MouseHook &mouseHook() { return d_mouseHook; }
		KeyboardHook &keyboardHook() { return d_keyboardHook; }
		TrayIconManager &trayIcon() { return d_trayIcon; }
		ConfigSheetManager &configDlg() { return d_configDlg; }

	private:

		Globals(Globals const &other); // not implemented
		Globals &operator=(Globals const &other); // not implemented

};

extern Globals *globals;

#endif
