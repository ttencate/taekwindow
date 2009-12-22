#include "imagelist.hpp"
#include "debuglog.hpp"

ImageList::ImageList(GdiPlusWrap &, size_t size)
:
	d_size(size),
	d_count(0),
	d_images(new Image*[size])
{
}

ImageList::~ImageList() {
	for (size_t i = 0; i < d_count; ++i) {
		delete d_images[i];
	}
	delete[] d_images;
}

void ImageList::load(int resourceId, int controlId) {
	ASSERT(d_count < d_size);
	d_images[d_count] = new Image(resourceId, controlId);
	++d_count;
}

Image *ImageList::forControl(int controlId) {
	for (size_t i = 0; i < d_count; ++i) {
		if (d_images[i]->controlId() == controlId)
			return d_images[i];
	}
	return NULL;
}
