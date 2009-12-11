#ifndef _CONFIG_HPP_
#define _CONFIG_HPP_

#include "types.hpp"

struct Configuration {

	/* Whether or not to show the system tray icon while the application is running.
	 */
	bool systemTrayIcon;

	/* Whether or not to launch at logon.
	 * Implemented by a shortcut in the Startup folder in the Start Menu.
	 */
	bool startAtLogon;

	/* The modifier key used for moving and resizing, as a virtual-key code.
	 * This is referred to as Modifier in the comments, but you can read Alt if you like.
	 */
	int modifier;

	/* The mouse buttons used for moving and resizing, respectively.
	 */
	MouseButton moveButton, resizeButton;

	/* The behaviour to use when resizing windows.
	 */
	ResizeMode resizeMode;

	/* The mouse button that, when clicked on the title bar, pushes the window to the back.
	 */
	MouseButton pushBackButton;

	/* The mouse wheel behaviour: scroll the window under the cursor (true), or the focused window (false)?
	 */
	bool scrollWindowUnderCursor;
	
	/* Sets all fields to their default values.
	 */
	void setDefaults();

};

// TODO these should be member functions

/* Loads the configuration from the registry/environment.
 * In case of missing values, fills in defaults.
 */
void loadConfig(Configuration *config);

/* Saves the configuration to the registry/environment.
 */
void saveConfig(Configuration *config);

#endif
