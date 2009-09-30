#ifndef _CONFIG_HPP_
#define _CONFIG_HPP_

#include "../shared/dllconfig.hpp"

struct EXEConfiguration {

	/* Whether or not to show the system tray icon while the application is running.
	 */
	bool systemTrayIcon;

	/* Whether or not to launch at logon.
	 * Implemented by a shortcut in the Startup folder in the Start Menu.
	 */
	bool startAtLogon;
	
	/* Sets all fields to their default values.
	 */
	void setDefaults();

};

void loadConfig(DLLConfiguration *dllConfig, EXEConfiguration *exeConfig);
void saveConfig(DLLConfiguration *dllConfig, EXEConfiguration *exeConfig);

#endif
