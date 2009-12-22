#ifndef IMAGELIST_HPP
#define IMAGELIST_HPP

#include "image.hpp"
#include "gdipluswrap.hpp"

class ImageList {

	size_t d_size;
	size_t d_count;
	Image **d_images;

	public:
		ImageList(GdiPlusWrap &gdiPlus, size_t size);
		~ImageList();

		void load(int resourceId, int controlId);
		Image *forControl(int controlId);

};

#endif
