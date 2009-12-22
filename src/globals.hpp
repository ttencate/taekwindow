#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include "config.hpp"
#include "wheelhandler.hpp"
#include "pushhandler.hpp"
#include "draghandler.hpp"
#include "handlerlist.hpp"
#include "configdlg.hpp"

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
	WheelHandler d_wheelHandler;
	PushHandler d_pushHandler;
	DragHandler d_dragHandler;
	MouseHandlerList d_mouseHandlerList;
	HHOOK d_llMouseHook;
	HHOOK d_llKeyboardHook;
	ConfigDlg d_configDlg;

	public:

		Globals();
		~Globals();

		Configuration &config() { return d_config; }
		HHOOK &llMouseHook() { return d_llMouseHook; }
		HHOOK &llKeyboardHook() { return d_llKeyboardHook; }
		MouseHandlerList &mouseHandlerList() { return d_mouseHandlerList; }
		ConfigDlg &configDlg() { return d_configDlg; }

	private:

		Globals(Globals const &other); // not implemented
		Globals &operator=(Globals const &other); // not implemented

};

extern Globals *globals;

#endif
