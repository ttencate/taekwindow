#include "main.hpp"
#include "util.hpp"
#include "config.hpp"
#include "configdlg.hpp"
#include "resource.h"
#include "version.h"
#include "picostdlib.h"

#include <windows.h>
#include <commctrl.h>
#include <tchar.h>
#include <gdiplus.h>

/* It ain't pretty, but it doesn't seem to be in standard headers.
 * Anyway, so much legacy code relies on this that
 * Microsoft will not change it in the near future.
 */
#define ID_APPLY_NOW 0x3021

/* Handle of the configuration dialog's window.
 */
HWND configWindowHandle = 0;

/* Original window procedure of the property sheet dialog.
 * We override this with our own, but call the original.
 */
WNDPROC origConfigWindowProc;

/* The currently active DLL configuration.
 * Only valid when the dialog is visible.
 */
DLLConfiguration newDllConfig;
EXEConfiguration newExeConfig;

/* GDI+ token.
 */
ULONG_PTR gdiplusToken;

/* The PNG images in the dialog.
 */
const int NUM_IMAGES = 5;
const int IMAGE_CONTROL_IDS[NUM_IMAGES] = { IDC_STARTUPIMAGE, IDC_SYSTRAYIMAGE, IDC_BOTTOMRIGHTIMAGE, IDC_NINERECTANGLESIMAGE, IDC_APPLOGO };
const int IMAGE_RESOURCE_IDS[NUM_IMAGES] = { IDB_STARTUP, IDB_TRAYICON, IDB_RESIZEBOTTOMRIGHT, IDB_RESIZENINERECTANGLES, IDB_LOGO };
Gdiplus::Bitmap *images[NUM_IMAGES];

void initDynamicLabels(HWND dialogHandle) {
	SetDlgItemText(dialogHandle, IDC_ABOUTGROUP, _T("About ") _T(APPLICATION_TITLE));
	SetDlgItemText(dialogHandle, IDC_APPTITLE, _T(APPLICATION_TITLE));
	SetDlgItemText(dialogHandle, IDC_APPVERSION, _T("Version ") _T(APPLICATION_VERSION_STRING));
	SetDlgItemText(dialogHandle, IDC_APPCOPYRIGHT, _T(APPLICATION_COPYRIGHT));
	SetDlgItemText(dialogHandle, IDC_APPEMAIL, _T("<a href=\"mailto:") _T(APPLICATION_EMAIL) _T("\">") _T(APPLICATION_EMAIL) _T("</a>"));
	SetDlgItemText(dialogHandle, IDC_APPWEBSITE, _T("<a href=\"") _T(APPLICATION_WEBSITE) _T("\">") _T(APPLICATION_WEBSITE) _T("</a>"));
	SetDlgItemText(dialogHandle, IDC_LICENSE, _T(APPLICATION_LICENSE_BRIEF) _T(" See the file <a href=\"") _T(APPLICATION_README_FILE) _T("\">") _T(APPLICATION_README_FILE) _T("</a> for details."));
}

void drawImage(DRAWITEMSTRUCT *item, Gdiplus::Bitmap *image) {
	HDC dc = item->hDC;
	Gdiplus::Graphics graphics(dc);

	image->SetResolution(graphics.GetDpiX(), graphics.GetDpiY());

	RECT &rect = item->rcItem;
	int x = (rect.left + rect.right - (int)image->GetWidth()) / 2;
	int y = (rect.top + rect.bottom - (int)image->GetHeight()) / 2;

	graphics.DrawImage(image, x, y);
}

void drawImageControl(int controlID, DRAWITEMSTRUCT *item) {
	for (int i = 0; i < NUM_IMAGES; ++i) {
		if (IMAGE_CONTROL_IDS[i] == controlID) {
			Gdiplus::Bitmap *image = images[i];
			if (image) {
				drawImage(item, image);
			}
			break;
		}
	}
}

void hyperlinkClicked(int controlID, NMLINK *nmLink) {
	LPCTSTR url = nmLink->item.szUrl;
	if (url) {
		if ((int)ShellExecute(NULL, _T("open"), url, NULL, NULL, SW_SHOWDEFAULT) <= 32) {
			switch (controlID) {
				case IDC_APPEMAIL:
					// N.B. skip "mailto:" in the string, start at 7th character (hackish).
					showError(configWindowHandle, _T("Could not launch e-mail program"), _T("It seems that your system does not have an e-mail program installed, or it is not set up properly to handle \"mailto:\" links.\r\n\r\nYou can still send e-mail to %1 manually."), &url[7]);
					break;
				case IDC_APPWEBSITE:
					showError(configWindowHandle, _T("Could not launch web browser"), _T("It seems that your system does not have a web browser installed, or it is not set up properly to handle \"http\" links.\r\n\r\nIf you do have a browser, point it to %1 manually."), url);
					break;
				case IDC_LICENSE:
					showError(configWindowHandle, _T("Could not find readme file"), _T("The file %1 could not be found in the current directory. You should have received this file along with %2.exe."), url, _T(MAIN_EXE_FILE));
					break;
				default:
					showLastError(configWindowHandle, _T("Could not open URL"));
			}
		}
	}
}

BOOL CALLBACK defaultDialogProc(HWND, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_INITDIALOG:
			return TRUE;
		case WM_DRAWITEM:
			drawImageControl((int)wParam, (DRAWITEMSTRUCT*)lParam);
			return TRUE;
		default:
			return FALSE;
	}
}

BOOL CALLBACK generalPageDialogProc(HWND dialogHandle, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_INITDIALOG:
			CheckDlgButton(dialogHandle, IDC_SYSTRAYICON, newExeConfig.systemTrayIcon);
			CheckDlgButton(dialogHandle, IDC_STARTATLOGON, newExeConfig.startAtLogon);
			break;
		case WM_COMMAND:
			PropSheet_Changed(configWindowHandle, dialogHandle);
			return TRUE;
		case WM_NOTIFY:
			switch (((NMHDR*)lParam)->code) {
				case PSN_APPLY:
					newExeConfig.systemTrayIcon = IsDlgButtonChecked(dialogHandle, IDC_SYSTRAYICON) == BST_CHECKED;
					newExeConfig.startAtLogon = IsDlgButtonChecked(dialogHandle, IDC_STARTATLOGON) == BST_CHECKED;
					return TRUE;
			}
			break;
	}
	return defaultDialogProc(dialogHandle, message, wParam, lParam);
}

void mutexButtonRadios(HWND dialogHandle, WPARAM wParam, int move, int resize) {
	if ((int)wParam == move && IsDlgButtonChecked(dialogHandle, resize)) {
		CheckRadioButton(dialogHandle, IDC_RESIZELEFT, IDC_RESIZERIGHT,
			resize == IDC_RESIZERIGHT ? IDC_RESIZEMIDDLE : IDC_RESIZERIGHT);
	}
	if ((int)wParam == resize && IsDlgButtonChecked(dialogHandle, move)) {
		CheckRadioButton(dialogHandle, IDC_MOVELEFT, IDC_MOVERIGHT,
			move == IDC_MOVELEFT ? IDC_MOVEMIDDLE : IDC_MOVELEFT);
	}
}

BOOL CALLBACK buttonsPageDialogProc(HWND dialogHandle, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_INITDIALOG:
			CheckRadioButton(dialogHandle, IDC_LEFTALT, IDC_RIGHTALT,
				newDllConfig.modifier == VK_MENU ? IDC_ANYALT :
				newDllConfig.modifier == VK_RMENU ? IDC_RIGHTALT :
				IDC_LEFTALT);
			CheckRadioButton(dialogHandle, IDC_MOVELEFT, IDC_MOVERIGHT,
				newDllConfig.moveButton == mbMiddle ? IDC_MOVEMIDDLE :
				newDllConfig.moveButton == mbRight ? IDC_MOVERIGHT :
				IDC_MOVELEFT);
			CheckRadioButton(dialogHandle, IDC_RESIZELEFT, IDC_RESIZERIGHT,
				newDllConfig.resizeButton == mbLeft ? IDC_RESIZELEFT :
				newDllConfig.resizeButton == mbMiddle ? IDC_RESIZEMIDDLE :
				IDC_RESIZERIGHT);
			break;
		case WM_COMMAND:
			// Prevent same button for different functions.
			mutexButtonRadios(dialogHandle, wParam, IDC_MOVELEFT  , IDC_RESIZELEFT  );
			mutexButtonRadios(dialogHandle, wParam, IDC_MOVEMIDDLE, IDC_RESIZEMIDDLE);
			mutexButtonRadios(dialogHandle, wParam, IDC_MOVERIGHT , IDC_RESIZERIGHT );
			PropSheet_Changed(configWindowHandle, dialogHandle);
			return TRUE;
		case WM_NOTIFY:
			switch (((NMHDR*)lParam)->code) {
				case PSN_APPLY:
					newDllConfig.modifier =
						IsDlgButtonChecked(dialogHandle, IDC_ANYALT) ? VK_MENU :
						IsDlgButtonChecked(dialogHandle, IDC_RIGHTALT) ? VK_RMENU :
						VK_LMENU;
					newDllConfig.moveButton =
						IsDlgButtonChecked(dialogHandle, IDC_MOVEMIDDLE) ? mbMiddle :
						IsDlgButtonChecked(dialogHandle, IDC_MOVERIGHT) ? mbRight :
						mbLeft;
					newDllConfig.resizeButton =
						IsDlgButtonChecked(dialogHandle, IDC_RESIZELEFT) ? mbLeft :
						IsDlgButtonChecked(dialogHandle, IDC_RESIZEMIDDLE) ? mbMiddle :
						mbRight;
					return TRUE;
			}
			break;
	}
	return defaultDialogProc(dialogHandle, message, wParam, lParam);
}

BOOL CALLBACK resizingPageDialogProc(HWND dialogHandle, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_INITDIALOG:
			CheckRadioButton(dialogHandle, IDC_BOTTOMRIGHT, IDC_NINERECTANGLES,
				newDllConfig.resizeMode == rmBottomRight ? IDC_BOTTOMRIGHT :
				IDC_NINERECTANGLES);
			break;
		case WM_COMMAND:
			PropSheet_Changed(configWindowHandle, dialogHandle);
			return TRUE;
		case WM_NOTIFY:
			switch (((NMHDR*)lParam)->code) {
				case PSN_APPLY:
					newDllConfig.resizeMode =
						IsDlgButtonChecked(dialogHandle, IDC_BOTTOMRIGHT) ? rmBottomRight :
						rmNineRectangles;
					return TRUE;
			}
			break;
	}
	return defaultDialogProc(dialogHandle, message, wParam, lParam);
}

BOOL CALLBACK scrollingPageDialogProc(HWND dialogHandle, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_INITDIALOG:
			CheckRadioButton(dialogHandle, IDC_SCROLLFOCUSED, IDC_SCROLLUNDERCURSOR,
				newDllConfig.scrollWindowUnderCursor ? IDC_SCROLLUNDERCURSOR : IDC_SCROLLFOCUSED);
			break;
		case WM_COMMAND:
			PropSheet_Changed(configWindowHandle, dialogHandle);
			return TRUE;
		case WM_NOTIFY:
			switch (((NMHDR*)lParam)->code) {
				case PSN_APPLY:
					newDllConfig.scrollWindowUnderCursor = IsDlgButtonChecked(dialogHandle, IDC_SCROLLUNDERCURSOR) == BST_CHECKED;
					return TRUE;
			}
			break;
	}
	return defaultDialogProc(dialogHandle, message, wParam, lParam);
}

BOOL CALLBACK aboutPageDialogProc(HWND dialogHandle, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_INITDIALOG:
			initDynamicLabels(dialogHandle);
			break;
		case WM_NOTIFY:
			NMHDR *nmHdr = (NMHDR*)lParam;
			switch (nmHdr->code) {
				case NM_CLICK:
				case NM_RETURN:
					hyperlinkClicked((int)wParam, (NMLINK*)lParam);
					return TRUE;
			}
			break;
	}
	return defaultDialogProc(dialogHandle, message, wParam, lParam);
}

/* The subclassed window procedure for the property sheet dialog.
 *
 * We need this because we want to apply the configuration *after*
 * all the pages have written to it.
 * The PropSheetProc can get us a notification (on Windows XP and above)
 * but it is sent *before* the pages get a PSN_APPLY notification,
 * so the configuration is not yet up to date at that point.
 */
LRESULT CALLBACK configWindowProc(HWND dialogHandle, UINT message, WPARAM wParam, LPARAM lParam) {
	LRESULT retVal = CallWindowProc(origConfigWindowProc, dialogHandle, message, wParam, lParam);
	switch (message) {
		case WM_COMMAND:
			switch (LOWORD(wParam)) {
				case IDOK:
				case ID_APPLY_NOW:
					applyConfig(&newDllConfig, &newExeConfig);
					saveConfig(&newDllConfig, &newExeConfig);
					break;
			}
			break;
	}	
	return retVal;
}

/* PropSheetProc for the configuration dialog.
 * Must return 0.
 */
int CALLBACK configPropSheetProc(HWND dialogHandle, UINT message, LPARAM) {
	switch (message) {
		case PSCB_INITIALIZED:
			configWindowHandle = dialogHandle;
			origConfigWindowProc = (WNDPROC)SetWindowLongPtr(dialogHandle, GWLP_WNDPROC, (LONG_PTR)&configWindowProc);
			break;
	}
	return 0;
}

/* Initializes the Common Controls library.
 * It is ok to call this multiple times, and they do not need shutdown.
 */
bool initCommonControls() {
	// Huhhuh, init common control sex. Allcaps break my head.
	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icc.dwICC = ICC_STANDARD_CLASSES | ICC_LINK_CLASS;
	if (InitCommonControlsEx(&icc) != TRUE) {
		showLastError(NULL, _T("Unable to initialize common controls"));
		return false;
	}
	return true;
}

/* Initializes GDI+.
 * Returns true on success.
 * If it returns false, the error will already have been reported to the user.
 */
bool initGdiplus() {
	// Assuming that 0 is an invalid token, but this is not documented...
	gdiplusToken = 0;

	Gdiplus::GdiplusStartupInput gsi;
	gsi.GdiplusVersion = 1;
	gsi.DebugEventCallback = NULL;
	gsi.SuppressBackgroundThread = FALSE;
	gsi.SuppressExternalCodecs = TRUE;
	Gdiplus::Status status = Gdiplus::GdiplusStartup(&gdiplusToken, &gsi, NULL);
	if (status != Gdiplus::Ok) {
		showError(NULL, _T("Could not initialize GDI+"), _T("The GDI+ graphics library could not be initialized. Some images will not be visible.\n\nGdiplusStartup returned the error code %1!d!."), status);
		return false;
	}
	return true;
}

void shutdownGdiplus() {
	// Assuming that 0 is an invalid token, but this is not documented...
	if (gdiplusToken) {
		Gdiplus::GdiplusShutdown(gdiplusToken);
	}
}

/* Loads the PNG images from resources, storing them in the images array.
 * Silently fails for images that cannot be loaded, storing NULL for that image.
 */
void loadImages() {
	for (int i = 0; i < NUM_IMAGES; ++i) {
		images[i] = NULL;

		// Find and load the resource from the executable.
		// It has the undocumented type "PNG".
		int resourceID = IMAGE_RESOURCE_IDS[i];
		HRSRC resourceHandle = FindResource(NULL, MAKEINTRESOURCE(resourceID), _T("PNG"));
		if (!resourceHandle) {
			debugShowLastError(_T("Could not find resource"));
			continue;
		}
		HGLOBAL resource = LoadResource(NULL, resourceHandle);
		if (!resource) {
			debugShowLastError(_T("Could not load resource"));
			continue;
		}
		void *resourceData = LockResource(resource);
		if (!resourceData) {
			debugShowLastError(_T("Could not lock resource"));
			continue;
		}

		// Allocate memory to copy the resource to.
		// Unfortunately, the HGLOBAL returned by LoadResource is not really a HGLOBAL,
		// so we cannot have CreateStreamOnHGlobal read from that. We have to make a copy.

		// Note: passing NULL to SizeofResource is not explicitly allowed by the docs,
		// but the other XxxResource functions do allow it, and much code on the internet also passes NULL.
		// So let's assume that this is an omission in the docs.
		DWORD bytes = SizeofResource(NULL, resourceHandle);
		if (!bytes) {
			debugShowLastError(_T("Could not determine resource size"));
			continue;
		}
		HGLOBAL mem = GlobalAlloc(GMEM_MOVEABLE, bytes);
		if (!mem) {
			debugShowLastError(_T("Could not allocate memory"));
			continue;
		}
		void *memData = GlobalLock(mem);
		if (!memData) {
			debugShowLastError(_T("Could not lock memory"));
			continue;
		}
		if (!memcpy(memData, resourceData, bytes)) {
			debugShowLastError(_T("Could not copy memory"));
			continue;
		}
		if (!GlobalUnlock(mem) && GetLastError()) {
			debugShowLastError(_T("Could not unlock memory"));
			continue;
		}

		// Create a stream from the memory.
		// Note that by passing TRUE, the stream calls GlobalFree automatically.
		IStream *stream;
		if (CreateStreamOnHGlobal(mem, TRUE, &stream) != S_OK) {
			debugShowLastError(_T("Could not create image stream"));
			GlobalFree(mem);
			continue;
		}
		// Stream might be bigger than resource, don't know whether PNG loader minds,
		// but better be safe.
		ULARGE_INTEGER largeBytes;
		largeBytes.HighPart = 0;
		largeBytes.LowPart = bytes;
		stream->SetSize(largeBytes);

		// Create an image from the stream.
		images[i] = new Gdiplus::Bitmap(stream);

		// Clean up.
		stream->Release();
	}
}

void unloadImages() {
	for (int i = 0; i < NUM_IMAGES; ++i) {
		if (images[i]) {
			delete images[i];
			images[i] = NULL;
		}
	}
}

INT_PTR showPropSheet() {
	// Most of the runs, the user will not use the configuration dialog.
	// That's why we do not initialize in WinMain, but only when it is used.
	if (!initCommonControls()) {
		return -1;
	}
	initGdiplus(); // we CAN work without GDI+, so ignore any errors
	loadImages();

	/* The property sheet pages in the configuration dialog.
	 */
	const int NUM_PAGES = 5;
	const int PAGE_TEMPLATES[NUM_PAGES] = { IDD_GENERALPAGE, IDD_BUTTONSPAGE, IDD_RESIZINGPAGE, IDD_SCROLLINGPAGE, IDD_ABOUTPAGE };
	const DLGPROC PAGE_DIALOG_PROCS[NUM_PAGES] = { &generalPageDialogProc, &buttonsPageDialogProc, &resizingPageDialogProc, &scrollingPageDialogProc, &aboutPageDialogProc };

	PROPSHEETPAGE pages[NUM_PAGES];
	for (int i = 0; i < NUM_PAGES; ++i) {
		PROPSHEETPAGE &page = pages[i];
		page.dwSize = sizeof(PROPSHEETPAGE);
		page.dwFlags = 0;
		page.hInstance = getCurrentInstance();
		page.pszTemplate = MAKEINTRESOURCE(PAGE_TEMPLATES[i]);
		page.hIcon = NULL;
		page.pszTitle = NULL; // fetched from dialog resource
		page.pfnDlgProc = PAGE_DIALOG_PROCS[i];
		page.lParam = 0;
		page.pfnCallback = NULL;
		page.pcRefParent = NULL;
		page.pszHeaderTitle = NULL;
		page.pszHeaderSubTitle = NULL;
	}

	PROPSHEETHEADER header;
	header.dwSize = sizeof(PROPSHEETHEADER);
	header.dwFlags = PSH_PROPSHEETPAGE | PSH_NOCONTEXTHELP | PSH_USEICONID | PSH_USECALLBACK;
	header.hwndParent = NULL;
	header.hInstance = getCurrentInstance();
	header.pszIcon = MAKEINTRESOURCE(IDI_APP);
	header.pszCaption = _T(APPLICATION_TITLE) _T(" Preferences");
	header.nPages = NUM_PAGES;
	header.nStartPage = 0;
	header.ppsp = pages;
	header.pfnCallback = &configPropSheetProc;
	header.hbmWatermark = NULL;
	header.pszbmWatermark = NULL;
	header.hplWatermark = NULL;
	header.hbmHeader = NULL;
	header.pszbmHeader = NULL;

	INT_PTR result = PropertySheet(&header);

	unloadImages();
	shutdownGdiplus();
	configWindowHandle = 0;
	return result;
}

void showConfig() {
	// Load the most current settings from the environment, in case they've changed since startup
	// by some external factor.
	loadConfig(&newDllConfig, &newExeConfig);

	// Show the dialog (modally).
	int result = showPropSheet();

	if (result >= 1) {
		// Changes were saved. They have already been applied;
		// now save them to the environment (registry, file system) as well.
		saveConfig(&newDllConfig, &newExeConfig);
	}
}

bool isConfigShowing() {
	return configWindowHandle != 0;
}

void focusConfig() {
	if (configWindowHandle) {
		BringWindowToTop(configWindowHandle);
	}
}