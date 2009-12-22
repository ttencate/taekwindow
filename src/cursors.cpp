#define OEMRESOURCE
#include <windows.h>
#include <tchar.h>

#include "cursors.hpp"

Cursors::Cursors() {
	d_cursors[crNormal] = load(OCR_NORMAL);
	d_cursors[crIgnore] = load(OCR_NO);
	d_cursors[crMove] = load(OCR_SIZEALL);
	d_cursors[crResizeNS] = load(OCR_SIZENS);
	d_cursors[crResizeWE] = load(OCR_SIZEWE);
	d_cursors[crResizeNWSE] = load(OCR_SIZENWSE);
	d_cursors[crResizeNESW] = load(OCR_SIZENESW);
}

HCURSOR Cursors::get(Cursor cursor) const {
	return d_cursors[cursor];
}

HCURSOR Cursors::load(int ocr) const {
	return (HCURSOR)LoadImage(NULL, MAKEINTRESOURCE(ocr), IMAGE_CURSOR, 0, 0, LR_SHARED);
}
