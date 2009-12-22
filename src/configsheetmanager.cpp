#include "configsheetmanager.hpp"
#include "globals.hpp"

ConfigSheetManager::ConfigSheetManager()
:
	d_sheet(NULL)
{
}

ConfigSheetManager::~ConfigSheetManager() {
	delete d_sheet;
}

void ConfigSheetManager::show() {
	if (d_sheet) {
		d_sheet->show();
	} else {
		d_sheet = new ConfigSheet(globals->config());
		d_sheet->show();
		delete d_sheet;
		d_sheet = NULL;
	}
}
