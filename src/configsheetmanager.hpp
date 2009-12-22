#ifndef CONFIGSHEETMANAGER_HPP
#define CONFIGSHEETMANAGER_HPP

#include "configsheet.hpp"

class ConfigSheetManager {

	ConfigSheet *d_sheet;

	public:

		ConfigSheetManager();
		~ConfigSheetManager();

		void show();

};

#endif
