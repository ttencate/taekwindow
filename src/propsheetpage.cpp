#include "propsheetpage.hpp"
#include "globals.hpp"

PropSheetPage::PropSheetPage(int templateId, DLGPROC dialogProc) {
	d_page.dwSize = sizeof(PROPSHEETPAGE);
	d_page.dwFlags = 0;
	d_page.hInstance = globals->currentInstance();
	d_page.pszTemplate = MAKEINTRESOURCE(templateId);
	d_page.hIcon = NULL;
	d_page.pszTitle = NULL; // fetched from dialog resource
	d_page.pfnDlgProc = dialogProc;
	d_page.lParam = 0;
	d_page.pfnCallback = NULL;
	d_page.pcRefParent = NULL;
	d_page.pszHeaderTitle = NULL;
	d_page.pszHeaderSubTitle = NULL;
}
