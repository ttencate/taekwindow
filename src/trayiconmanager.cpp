#include "trayiconmanager.hpp"
#include "globals.hpp"

TrayIconManager::TrayIconManager()
:
	d_trayIcon(NULL)
{
}

TrayIconManager::~TrayIconManager() {
	if (d_trayIcon)
		delete d_trayIcon;
}

void TrayIconManager::update() {
	if (globals->config().systemTrayIcon && !d_trayIcon) {
		d_trayIcon = new TrayIcon();
	} else if (!globals->config().systemTrayIcon && d_trayIcon) {
		delete d_trayIcon;
		d_trayIcon = NULL;
	}
	if (d_trayIcon)
		d_trayIcon->update();
}
