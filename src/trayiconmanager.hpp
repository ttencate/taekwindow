#ifndef TRAYICONMANAGER_HPP
#define TRAYICONMANAGER_HPP

#include "trayicon.hpp"

class TrayIconManager {

	TrayIcon *d_trayIcon;

	public:

		TrayIconManager();
		~TrayIconManager();

		/* Updates the tray icon with the current status.
		 */
		void update();

};

#endif
