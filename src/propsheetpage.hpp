#ifndef PROPSHEETPAGE_HPP
#define PROPSHEETPAGE_HPP

#include <windows.h>

class PropSheetPage {

	PROPSHEETPAGE d_page;

	public:

		PropSheetPage(int templateId, DLGPROC dialogProc);

		PROPSHEETPAGE const &asStruct() const { return d_page; }

};

#endif
