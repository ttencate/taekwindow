#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <windows.h>
#include <gdiplus.h>

/* Wrapper for a GDI+ image.
 * Assumes that the GDI+ exists during its lifetime.
 */
class Image {

	Gdiplus::Bitmap *d_bitmap;
	int d_controlId;

	public:

		Image(int resourceId, int controlId);
		~Image();

		int controlId() const { return d_controlId; }
		void draw(DRAWITEMSTRUCT const &item) const;

};

#endif
