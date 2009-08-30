#include "main.hpp"
#include "util.hpp"
#include "configdlg.hpp"
#include "resource.h"
#include "configdlgres.h"
#include "version.h"

#include <windows.h>
#include <commctrl.h>

/* Handle of the configuration dialog's window.
 */
HWND configWindowHandle = 0;

BOOL CALLBACK defaultDialogProc(HWND dialogHandle, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_INITDIALOG:
			return TRUE;
		default:
			return FALSE;
	}
}

BOOL CALLBACK aboutPageDialogProc(HWND dialogHandle, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_COMMAND:
			// TODO process hyperlink click
			return TRUE;
		default:
			return defaultDialogProc(dialogHandle, message, wParam, lParam);
	}
}

void showConfig() {
	// Huhhuh, init common control sex. Allcaps break my head.
	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icc.dwICC = ICC_STANDARD_CLASSES; // TODO ICC_LINK_CLASS
	InitCommonControlsEx(&icc);

	const int NUM_PAGES = 1;
	const int pageTemplates[NUM_PAGES] = { IDD_ABOUTPAGE };
	const DLGPROC pageDialogProcs[NUM_PAGES] = { &aboutPageDialogProc };

	PROPSHEETPAGE pages[NUM_PAGES];
	for (int i = 0; i < NUM_PAGES; ++i) {
		PROPSHEETPAGE &page = pages[i];
		page.dwSize = sizeof(PROPSHEETPAGE);
		page.dwFlags = 0;
		page.hInstance = getCurrentInstance();
		page.pszTemplate = MAKEINTRESOURCE(pageTemplates[i]);
		page.hIcon = NULL;
		page.pszTitle = NULL; // fetched from dialog resource
		page.pfnDlgProc = pageDialogProcs[i];
		page.lParam = 0;
		page.pfnCallback = NULL;
		page.pcRefParent = NULL;
		page.pszHeaderTitle = NULL;
		page.pszHeaderSubTitle = NULL;
	}

	PROPSHEETHEADER header;
	header.dwSize = sizeof(PROPSHEETHEADER);
	header.dwFlags = PSH_PROPSHEETPAGE | PSH_NOCONTEXTHELP | PSH_USEICONID;
	header.hwndParent = NULL;
	header.hInstance = getCurrentInstance();
	header.pszIcon = MAKEINTRESOURCE(APPICON);
	header.pszCaption = APPLICATION_TITLE_W L" configuration";
	header.nPages = NUM_PAGES;
	header.nStartPage = 0;
	header.ppsp = pages;
	header.pfnCallback = NULL;
	header.hbmWatermark = NULL;
	header.pszbmWatermark = NULL;
	header.hplWatermark = NULL;
	header.hbmHeader = NULL;
	header.pszbmHeader = NULL;

	PropertySheet(&header);
}

bool isConfigShowing() {
	return configWindowHandle != 0;
}

void focusConfig() {
	if (configWindowHandle) {
		BringWindowToTop(configWindowHandle);
	}
}
