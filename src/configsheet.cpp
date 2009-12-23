#include <tchar.h>

#include "configsheet.hpp"
#include "main.hpp"
#include "debug.hpp"
#include "errors.hpp"
#include "resource.h"
#include "version.hpp"
#include "picostdlib.h"

/* It ain't pretty, but it doesn't seem to be in standard headers.
 * Anyway, so much legacy code relies on this that
 * Microsoft will not change it in the near future.
 */
#define ID_APPLY_NOW 0x3021

size_t const ConfigSheet::s_numPages = 5;
int const ConfigSheet::s_templateIds[ConfigSheet::s_numPages] = { IDD_GENERALPAGE, IDD_BUTTONSPAGE, IDD_RESIZINGPAGE, IDD_SCROLLINGPAGE, IDD_ABOUTPAGE };
DLGPROC const ConfigSheet::s_pageProcs[ConfigSheet::s_numPages] = { &ConfigSheet::generalPageFwd, &ConfigSheet::buttonsPageFwd, &ConfigSheet::resizingPageFwd, &ConfigSheet::scrollingPageFwd, &ConfigSheet::aboutPageFwd };

size_t const ConfigSheet::s_numImages = 5;
int const ConfigSheet::s_resourceIds[ConfigSheet::s_numImages] = { IDB_STARTUP, IDB_TRAYICON, IDB_RESIZEBOTTOMRIGHT, IDB_RESIZENINERECTANGLES, IDB_LOGO };
int const ConfigSheet::s_controlIds[ConfigSheet::s_numImages] = { IDC_STARTUPIMAGE, IDC_SYSTRAYIMAGE, IDC_BOTTOMRIGHTIMAGE, IDC_NINERECTANGLESIMAGE, IDC_APPLOGO };

ConfigSheet *ConfigSheet::s_instance = NULL;

ConfigSheet::ConfigSheet(Configuration const &config)
:
	PropSheet(_T(APPLICATION_TITLE) _T(" Preferences"), LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_APP))),
	d_newConfig(config),
	d_origWindowProc(NULL),
	d_gdiPlus(),
	d_imageList(d_gdiPlus, s_numImages),
	d_handle(NULL)
{
	ASSERT(!s_instance);
	s_instance = this;

	for (size_t i = 0; i < s_numImages; ++i) {
		d_imageList.load(s_resourceIds[i], s_controlIds[i]);
	}

	for (size_t i = 0; i < s_numPages; ++i) {
		addPage(PropSheetPage(s_templateIds[i], s_pageProcs[i]));
	}
}

ConfigSheet::~ConfigSheet() {
	s_instance = NULL;
}

void ConfigSheet::show() {
	if (isShowing()) {
		bringToFront();
	} else {
		showModal();
	}
}

bool ConfigSheet::isShowing() {
	return d_handle != 0;
}

BOOL CALLBACK ConfigSheet::generalPageFwd(HWND dialogHandle, UINT message, WPARAM wParam, LPARAM lParam) {
	ASSERT(s_instance);
	return s_instance->generalPageDialogProc(dialogHandle, message, wParam, lParam);
}

BOOL CALLBACK ConfigSheet::buttonsPageFwd(HWND dialogHandle, UINT message, WPARAM wParam, LPARAM lParam) {
	ASSERT(s_instance);
	return s_instance->buttonsPageDialogProc(dialogHandle, message, wParam, lParam);
}

BOOL CALLBACK ConfigSheet::resizingPageFwd(HWND dialogHandle, UINT message, WPARAM wParam, LPARAM lParam) {
	ASSERT(s_instance);
	return s_instance->resizingPageDialogProc(dialogHandle, message, wParam, lParam);
}

BOOL CALLBACK ConfigSheet::scrollingPageFwd(HWND dialogHandle, UINT message, WPARAM wParam, LPARAM lParam) {
	ASSERT(s_instance);
	return s_instance->scrollingPageDialogProc(dialogHandle, message, wParam, lParam);
}

BOOL CALLBACK ConfigSheet::aboutPageFwd(HWND dialogHandle, UINT message, WPARAM wParam, LPARAM lParam) {
	ASSERT(s_instance);
	return s_instance->aboutPageDialogProc(dialogHandle, message, wParam, lParam);
}

LRESULT CALLBACK ConfigSheet::configFwd(HWND dialogHandle, UINT message, WPARAM wParam, LPARAM lParam) {
	ASSERT(s_instance);
	return s_instance->configWindowProc(dialogHandle, message, wParam, lParam);
}

void ConfigSheet::initDynamicLabels(HWND pageHandle) {
	SetDlgItemText(pageHandle, IDC_ABOUTGROUP, _T("About ") _T(APPLICATION_TITLE));
	SetDlgItemText(pageHandle, IDC_APPTITLE, _T(APPLICATION_TITLE));
	SetDlgItemText(pageHandle, IDC_APPVERSION, _T("Version ") _T(APPLICATION_VERSION_STRING));
	SetDlgItemText(pageHandle, IDC_APPCOPYRIGHT, _T(APPLICATION_COPYRIGHT));
	SetDlgItemText(pageHandle, IDC_APPEMAIL, _T("<a href=\"mailto:") _T(APPLICATION_EMAIL) _T("\">") _T(APPLICATION_EMAIL) _T("</a>"));
	SetDlgItemText(pageHandle, IDC_APPWEBSITE, _T("<a href=\"") _T(APPLICATION_WEBSITE) _T("\">") _T(APPLICATION_WEBSITE) _T("</a>"));
	SetDlgItemText(pageHandle, IDC_LICENSE, _T(APPLICATION_LICENSE_BRIEF) _T(" See the file <a href=\"") _T(APPLICATION_README_FILE) _T("\">") _T(APPLICATION_README_FILE) _T("</a> for details."));
}

void ConfigSheet::drawImageControl(int controlId, DRAWITEMSTRUCT const &item) {
	Image *image = d_imageList.forControl(controlId);
	if (image)
		image->draw(item);
}

void ConfigSheet::hyperlinkClicked(int controlID, NMLINK const &nmLink) {
	LPCTSTR url = nmLink.item.szUrl;
	if (url) {
		if ((int)ShellExecute(NULL, _T("open"), url, NULL, NULL, SW_SHOWDEFAULT) <= 32) {
			switch (controlID) {
				case IDC_APPEMAIL:
					// N.B. skip "mailto:" in the string, start at 7th character (hackish).
					showError(d_handle, _T("Could not launch e-mail program"), _T("It seems that your system does not have an e-mail program installed, or it is not set up properly to handle \"mailto:\" links.\r\n\r\nYou can still send e-mail to %1 manually."), &url[7]);
					break;
				case IDC_APPWEBSITE:
					showError(d_handle, _T("Could not launch web browser"), _T("It seems that your system does not have a web browser installed, or it is not set up properly to handle \"http\" links.\r\n\r\nIf you do have a browser, point it to %1 manually."), url);
					break;
				case IDC_LICENSE:
					showError(d_handle, _T("Could not find readme file"), _T("The file %1 could not be found in the current directory. You should have received this file along with %2.exe."), url, _T(MAIN_EXE_FILE));
					break;
				default:
					showLastError(d_handle, _T("Could not open URL"));
			}
		}
	}
}

void ConfigSheet::mutexButtonRadios(HWND pageHandle, WPARAM wParam, int move, int resize) {
	if ((int)wParam == move && IsDlgButtonChecked(pageHandle, resize)) {
		CheckRadioButton(pageHandle, IDC_RESIZELEFT, IDC_RESIZERIGHT,
			resize == IDC_RESIZERIGHT ? IDC_RESIZEMIDDLE : IDC_RESIZERIGHT);
	}
	if ((int)wParam == resize && IsDlgButtonChecked(pageHandle, move)) {
		CheckRadioButton(pageHandle, IDC_MOVELEFT, IDC_MOVERIGHT,
			move == IDC_MOVELEFT ? IDC_MOVEMIDDLE : IDC_MOVELEFT);
	}
}

BOOL ConfigSheet::commonDialogProc(HWND, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_INITDIALOG:
			return TRUE;
		case WM_DRAWITEM:
			s_instance->drawImageControl((int)wParam, *(DRAWITEMSTRUCT*)lParam);
			return TRUE;
		default:
			return FALSE;
	}
}

BOOL ConfigSheet::generalPageDialogProc(HWND pageHandle, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_INITDIALOG:
			CheckDlgButton(pageHandle, IDC_SYSTRAYICON, d_newConfig.systemTrayIcon);
			CheckDlgButton(pageHandle, IDC_STARTATLOGON, d_newConfig.startAtLogon);
			break;
		case WM_COMMAND:
			switch (LOWORD(wParam)) {
				case IDC_EXITBUTTON:
					PostQuitMessage(0);
					break;
				default:
					PropSheet_Changed(d_handle, pageHandle);
					break;
			}
			return TRUE;
		case WM_NOTIFY:
			switch (((NMHDR*)lParam)->code) {
				case PSN_APPLY:
					d_newConfig.systemTrayIcon = IsDlgButtonChecked(pageHandle, IDC_SYSTRAYICON) == BST_CHECKED;
					d_newConfig.startAtLogon = IsDlgButtonChecked(pageHandle, IDC_STARTATLOGON) == BST_CHECKED;
					return TRUE;
			}
			break;
	}
	return commonDialogProc(pageHandle, message, wParam, lParam);
}

BOOL ConfigSheet::buttonsPageDialogProc(HWND pageHandle, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_INITDIALOG:
			CheckRadioButton(pageHandle, IDC_LEFTALT, IDC_RIGHTALT,
				d_newConfig.modifier == VK_MENU ? IDC_ANYALT :
				d_newConfig.modifier == VK_RMENU ? IDC_RIGHTALT :
				IDC_LEFTALT);
			CheckRadioButton(pageHandle, IDC_MOVELEFT, IDC_MOVERIGHT,
				d_newConfig.moveButton == mbMiddle ? IDC_MOVEMIDDLE :
				d_newConfig.moveButton == mbRight ? IDC_MOVERIGHT :
				IDC_MOVELEFT);
			CheckRadioButton(pageHandle, IDC_RESIZELEFT, IDC_RESIZERIGHT,
				d_newConfig.resizeButton == mbLeft ? IDC_RESIZELEFT :
				d_newConfig.resizeButton == mbMiddle ? IDC_RESIZEMIDDLE :
				IDC_RESIZERIGHT);
			CheckDlgButton(pageHandle, IDC_PUSHBACKCHECK, d_newConfig.pushBackButton == mbMiddle);
			break;
		case WM_COMMAND:
			// Prevent same button for different functions.
			mutexButtonRadios(pageHandle, wParam, IDC_MOVELEFT  , IDC_RESIZELEFT  );
			mutexButtonRadios(pageHandle, wParam, IDC_MOVEMIDDLE, IDC_RESIZEMIDDLE);
			mutexButtonRadios(pageHandle, wParam, IDC_MOVERIGHT , IDC_RESIZERIGHT );
			PropSheet_Changed(d_handle, pageHandle);
			return TRUE;
		case WM_NOTIFY:
			switch (((NMHDR*)lParam)->code) {
				case PSN_APPLY:
					d_newConfig.modifier =
						IsDlgButtonChecked(pageHandle, IDC_ANYALT) ? VK_MENU :
						IsDlgButtonChecked(pageHandle, IDC_RIGHTALT) ? VK_RMENU :
						VK_LMENU;
					d_newConfig.moveButton =
						IsDlgButtonChecked(pageHandle, IDC_MOVEMIDDLE) ? mbMiddle :
						IsDlgButtonChecked(pageHandle, IDC_MOVERIGHT) ? mbRight :
						mbLeft;
					d_newConfig.resizeButton =
						IsDlgButtonChecked(pageHandle, IDC_RESIZELEFT) ? mbLeft :
						IsDlgButtonChecked(pageHandle, IDC_RESIZEMIDDLE) ? mbMiddle :
						mbRight;
					d_newConfig.pushBackButton =
						IsDlgButtonChecked(pageHandle, IDC_PUSHBACKCHECK) ? mbMiddle : mbNone;
					return TRUE;
			}
			break;
	}
	return commonDialogProc(pageHandle, message, wParam, lParam);
}

BOOL ConfigSheet::resizingPageDialogProc(HWND pageHandle, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_INITDIALOG:
			CheckRadioButton(pageHandle, IDC_BOTTOMRIGHT, IDC_NINERECTANGLES,
				d_newConfig.resizeMode == rmBottomRight ? IDC_BOTTOMRIGHT : IDC_NINERECTANGLES);
			break;
		case WM_COMMAND:
			PropSheet_Changed(d_handle, pageHandle);
			return TRUE;
		case WM_NOTIFY:
			switch (((NMHDR*)lParam)->code) {
				case PSN_APPLY:
					d_newConfig.resizeMode = IsDlgButtonChecked(pageHandle, IDC_BOTTOMRIGHT) ? rmBottomRight : rmNineRectangles;
					return TRUE;
			}
			break;
	}
	return commonDialogProc(pageHandle, message, wParam, lParam);
}

BOOL ConfigSheet::scrollingPageDialogProc(HWND pageHandle, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_INITDIALOG:
			CheckRadioButton(pageHandle, IDC_SCROLLFOCUSED, IDC_SCROLLUNDERCURSOR,
				d_newConfig.scrollWindowUnderCursor ? IDC_SCROLLUNDERCURSOR : IDC_SCROLLFOCUSED);
			break;
		case WM_COMMAND:
			PropSheet_Changed(d_handle, pageHandle);
			return TRUE;
		case WM_NOTIFY:
			switch (((NMHDR*)lParam)->code) {
				case PSN_APPLY:
					d_newConfig.scrollWindowUnderCursor = IsDlgButtonChecked(pageHandle, IDC_SCROLLUNDERCURSOR) == BST_CHECKED;
					return TRUE;
			}
			break;
	}
	return commonDialogProc(pageHandle, message, wParam, lParam);
}

BOOL ConfigSheet::aboutPageDialogProc(HWND pageHandle, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_INITDIALOG:
			initDynamicLabels(pageHandle);
			break;
		case WM_NOTIFY:
			NMHDR *nmHdr = (NMHDR*)lParam;
			switch (nmHdr->code) {
				case NM_CLICK:
				case NM_RETURN:
					hyperlinkClicked((int)wParam, *(NMLINK*)lParam);
					return TRUE;
			}
			break;
	}
	return commonDialogProc(pageHandle, message, wParam, lParam);
}

/* The subclassed window procedure for the property sheet dialog.
 *
 * We need this because we want to apply the configuration *after*
 * all the pages have written to it.
 * The PropSheetProc can get us a notification (on Windows XP and above)
 * but it is sent *before* the pages get a PSN_APPLY notification,
 * so the d_newConfig is not yet up to date at that point.
 */
LRESULT ConfigSheet::configWindowProc(HWND pageHandle, UINT message, WPARAM wParam, LPARAM lParam) {
	LRESULT retVal = CallWindowProc(d_origWindowProc, pageHandle, message, wParam, lParam);
	switch (message) {
		case WM_COMMAND:
			switch (LOWORD(wParam)) {
				case IDOK:
				case ID_APPLY_NOW:
					applyConfig(d_newConfig);
					d_newConfig.save();
					break;
			}
			break;
	}	
	return retVal;
}

/* PropSheetProc callback for the configuration dialog.
 */
void ConfigSheet::callback(HWND pageHandle, UINT message, LPARAM) {
	switch (message) {
		case PSCB_INITIALIZED:
			d_handle = pageHandle;
			d_origWindowProc = (WNDPROC)SetWindowLongPtr(pageHandle, GWLP_WNDPROC, (LONG_PTR)&configFwd);
			break;
	}
}

void ConfigSheet::bringToFront() {
	if (isShowing()) {
		BringWindowToTop(d_handle);
	}
}
