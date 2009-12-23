#include <tchar.h>

#include "image.hpp"
#include "errors.hpp"
#include "debug.hpp"
#include "picostdlib.h"

Image::Image(int resourceId, int controlId)
:
	d_image(NULL),
	d_controlId(controlId)
{
	// Find and load the resource from the executable.
	// It has the undocumented type "PNG".
	HRSRC resourceHandle = FindResource(NULL, MAKEINTRESOURCE(resourceId), _T("PNG"));
	if (!resourceHandle) {
		debugShowLastError("Could not find resource");
		return;
	}
	HGLOBAL resource = LoadResource(NULL, resourceHandle);
	if (!resource) {
		debugShowLastError("Could not load resource");
		return;
	}
	void *resourceData = LockResource(resource);
	if (!resourceData) {
		debugShowLastError("Could not lock resource");
		return;
	}

	// Allocate memory to copy the resource to.
	// Unfortunately, the HGLOBAL returned by LoadResource is not really a HGLOBAL,
	// so we cannot have CreateStreamOnHGlobal read from that. We have to make a copy.

	// Note: passing NULL to SizeofResource is not explicitly allowed by the docs,
	// but the other XxxResource functions do allow it, and much code on the internet also passes NULL.
	// So let's assume that this is an omission in the docs.
	DWORD bytes = SizeofResource(NULL, resourceHandle);
	if (!bytes) {
		debugShowLastError("Could not determine resource size");
		return;
	}
	HGLOBAL mem = GlobalAlloc(GMEM_MOVEABLE, bytes);
	if (!mem) {
		debugShowLastError("Could not allocate memory");
		return;
	}
	void *memData = GlobalLock(mem);
	if (!memData) {
		debugShowLastError("Could not lock memory");
		return;
	}
	if (!memcpy(memData, resourceData, bytes)) {
		debugShowLastError("Could not copy memory");
		return;
	}
	if (!GlobalUnlock(mem) && GetLastError()) {
		debugShowLastError("Could not unlock memory");
		return;
	}

	// Create a stream from the memory.
	// Note that by passing TRUE, the stream calls GlobalFree automatically.
	IStream *stream;
	if (CreateStreamOnHGlobal(mem, TRUE, &stream) != S_OK) {
		debugShowLastError("Could not create image stream");
		GlobalFree(mem);
		return;
	}
	// Stream might be bigger than resource, don't know whether PNG loader minds,
	// but better be safe.
	ULARGE_INTEGER largeBytes;
	largeBytes.HighPart = 0;
	largeBytes.LowPart = bytes;
	stream->SetSize(largeBytes);

	// Create an image from the stream.
	d_image = new Gdiplus::Image(stream);
	if (!d_image) {
		debugShowLastError("Could not create GDI+ Bitmap from image stream");
	}

	// Clean up.
	stream->Release();
}

Image::~Image() {
	delete d_image;
}

void Image::draw(DRAWITEMSTRUCT const &item) const {
	if (!d_image)
		return;

	HDC dc = item.hDC;
	Gdiplus::Graphics graphics(dc);

	RECT const &rect = item.rcItem;
	DEBUGLOG("Drawing to rectangle %d, %d, %d, %d", rect.left, rect.top, rect.right, rect.bottom);
	int x = (rect.left + rect.right - (int)d_image->GetWidth()) / 2;
	int y = (rect.top + rect.bottom - (int)d_image->GetHeight()) / 2;
	DEBUGLOG("Drawing at %d, %d", x, y);

	Gdiplus::Rect imgRect(x, y, d_image->GetWidth(), d_image->GetHeight());

	graphics.DrawImage(d_image, imgRect);
}