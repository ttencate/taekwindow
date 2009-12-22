#include <windows.h>
#include <tchar.h>

#include "gdipluswrap.hpp"
#include "errors.hpp"

GdiPlusWrap::GdiPlusWrap()
:
	d_token(0),
	d_ok(false)
{
	Gdiplus::GdiplusStartupInput gsi;
	gsi.GdiplusVersion = 1;
	gsi.DebugEventCallback = NULL;
	gsi.SuppressBackgroundThread = FALSE;
	gsi.SuppressExternalCodecs = TRUE;
	Gdiplus::Status status = Gdiplus::GdiplusStartup(&d_token, &gsi, NULL);
	if (status != Gdiplus::Ok) {
		showError(NULL, _T("Could not initialize GDI+"), _T("The GDI+ graphics library could not be initialized. Some images will not be visible.\n\nGdiplusStartup returned the error code %1!d!."), status);
		return;
	}
	d_ok = true;
}

GdiPlusWrap::~GdiPlusWrap() {
	if (d_ok) {
		Gdiplus::GdiplusShutdown(d_token);
	}
}
