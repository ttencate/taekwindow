#ifndef _CONFIG_HPP_
#define _CONFIG_HPP_

#include "../shared/dllconfig.hpp"

class EXEConfiguration {

	public:

		/* Whether or not to show the system tray icon while the application is running.
		 */
		bool systemTrayIcon;
		
		/* Sets all fields to their default values.
		 */
		void setDefaults();
};

extern EXEConfiguration config;

/* Reads the configuration settings from the registry and places them in the objects pointed to.
 */
void readConfigFromRegistry(DLLConfiguration *dllconfig, EXEConfiguration *execonfig);

#endif
