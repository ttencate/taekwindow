#ifndef TRAYICONCONTROLLER_HPP
#define TRAYICONCONTROLLER_HPP

#include "trayicon.hpp"

class TrayIconController {

	TrayIcon *d_trayIcon;

	public:

		TrayIconController();
		~TrayIconController();

		/* Updates the tray icon with the current status.
		 */
		void update();

};

#endif
