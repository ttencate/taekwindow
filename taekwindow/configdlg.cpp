#include "main.hpp"
#include "util.hpp"
#include "configdlg.hpp"
#include "resource.h"
#include "version.h"

#include <windows.h>
#include <commctrl.h>
#include <tchar.h>
#include <gdiplus.h>

// TODO remove
#include <stdio.h>

/* Handle of the configuration dialog's window.
 */
HWND configWindowHandle = 0;

/* GDI+ token.
 */
ULONG_PTR gdiplusToken;

/* The PNG images in the dialog.
 */
const int NUM_IMAGES = 5;
const int IMAGE_CONTROL_IDS[NUM_IMAGES] = { IDC_STARTUPIMAGE, IDC_SYSTRAYIMAGE, 0, 0, IDC_APPLOGO };
const int IMAGE_RESOURCE_IDS[NUM_IMAGES] = { IDB_STARTUP, IDB_TRAYICON, IDB_RESIZEBOTTOMRIGHT, IDB_RESIZENINERECTANGLES, IDB_LOGO };
Gdiplus::Bitmap *images[NUM_IMAGES];

void initDynamicLabels(HWND dialogHandle) {
	SetDlgItemText(dialogHandle, IDC_ABOUTGROUP, _T("About ") _T(APPLICATION_TITLE));
	SetDlgItemText(dialogHandle, IDC_APPTITLE, _T(APPLICATION_TITLE));
	SetDlgItemText(dialogHandle, IDC_APPVERSION, _T("Version ") _T(APPLICATION_VERSION_STRING));
	SetDlgItemText(dialogHandle, IDC_APPCOPYRIGHT, _T(APPLICATION_COPYRIGHT));
	SetDlgItemText(dialogHandle, IDC_APPEMAIL, _T("<a>") _T(APPLICATION_EMAIL) _T("</a>"));
	SetDlgItemText(dialogHandle, IDC_APPWEBSITE, _T("<a>") _T(APPLICATION_WEBSITE) _T("</a>"));
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

void hyperlinkClicked(LONG_PTR controlID) {
	// TODO
}

BOOL CALLBACK defaultDialogProc(HWND dialogHandle, UINT message, WPARAM wParam, LPARAM lParam) {
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

BOOL CALLBACK aboutPageDialogProc(HWND dialogHandle, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_INITDIALOG:
			initDynamicLabels(dialogHandle);
			break;
		case WM_COMMAND:
			hyperlinkClicked(GetWindowLongPtr(dialogHandle, GWLP_ID));
			return TRUE;
	}
	return defaultDialogProc(dialogHandle, message, wParam, lParam);
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
		showLastError(_T("Unable to initialize common controls"));
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
		showError(_T("Could not initialize GDI+"), _T("The GDI+ graphics library could not be initialized. Some images will not be visible.\n\nGdiplusStartup returned the error code %1!d!."), status);
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
		HRSRC resourceHandle = FindResource(NULL, MAKEINTRESOURCE(resourceID), L"PNG");
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
		if (!CopyMemory(memData, resourceData, bytes)) {
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

void showConfig() {
	// Most of the runs, the user will not use the configuration dialog.
	// That's why we do not initialize in WinMain, but only when it is used.
	if (!initCommonControls()) {
		return;
	}
	initGdiplus(); // we CAN work without GDI+, so ignore any errors
	loadImages();

	/* The property sheet pages in the configuration dialog.
	 */
	const int NUM_PAGES = 5;
	const int PAGE_TEMPLATES[NUM_PAGES] = { IDD_GENERALPAGE, IDD_BUTTONSPAGE, IDD_RESIZINGPAGE, IDD_SCROLLINGPAGE, IDD_ABOUTPAGE };
	const DLGPROC PAGE_DIALOG_PROCS[NUM_PAGES] = { &defaultDialogProc, &defaultDialogProc, &defaultDialogProc, &defaultDialogProc, &aboutPageDialogProc };

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
	header.dwFlags = PSH_PROPSHEETPAGE | PSH_NOCONTEXTHELP | PSH_USEICONID;
	header.hwndParent = NULL;
	header.hInstance = getCurrentInstance();
	header.pszIcon = MAKEINTRESOURCE(IDI_APP);
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

	unloadImages();
	shutdownGdiplus();
}

bool isConfigShowing() {
	return configWindowHandle != 0;
}

void focusConfig() {
	if (configWindowHandle) {
		BringWindowToTop(configWindowHandle);
	}
}
