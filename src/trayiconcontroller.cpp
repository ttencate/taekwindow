#include "trayiconcontroller.hpp"
#include "globals.hpp"

TrayIconController::TrayIconController()
:
	d_trayIcon(NULL)
{
}

TrayIconController::~TrayIconController() {
	if (d_trayIcon)
		delete d_trayIcon;
}

void TrayIconController::update() {
	if (globals->config().systemTrayIcon && !d_trayIcon) {
		d_trayIcon = new TrayIcon();
	} else if (!globals->config().systemTrayIcon && d_trayIcon) {
		delete d_trayIcon;
		d_trayIcon = NULL;
	}
	if (d_trayIcon)
		d_trayIcon->update();
}
