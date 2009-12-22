#include "propsheet.hpp"
#include "debug.hpp"

PropSheet *PropSheet::s_instance = NULL;

PropSheet::PropSheet(TCHAR const *caption, HICON icon) {
	d_header.dwSize = sizeof(PROPSHEETHEADER);
	d_header.dwFlags = PSH_PROPSHEETPAGE | PSH_NOCONTEXTHELP | PSH_USEHICON | PSH_USECALLBACK;
	d_header.hwndParent = NULL;
	d_header.hInstance = NULL; // not needed; we don't load anything from the instance
	d_header.hIcon = icon; // if we use pszIcon instead, the Win7 taskbar button gets the 16x16 icon...
	d_header.pszCaption = caption;
	d_header.nPages = 0;
	d_header.nStartPage = 0;
	d_header.ppsp = d_pages;
	d_header.pfnCallback = &propSheetProc;
	d_header.hbmWatermark = NULL;
	d_header.pszbmWatermark = NULL;
	d_header.hplWatermark = NULL;
	d_header.hbmHeader = NULL;
	d_header.pszbmHeader = NULL;
}

void PropSheet::addPage(PropSheetPage const &page) {
	ASSERT(d_header.nPages <= s_maxNumPages);
	d_pages[d_header.nPages] = page.asStruct();
	++d_header.nPages;
}

INT_PTR PropSheet::showModal() {
	s_instance = this;
	return PropertySheet(&d_header);
}

int CALLBACK PropSheet::propSheetProc(HWND hwnd, UINT msg, LPARAM lParam) {
	s_instance->callback(hwnd, msg, lParam);
	return 0;
}

void PropSheet::callback(HWND, UINT, LPARAM) {
}