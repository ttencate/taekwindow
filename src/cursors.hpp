#ifndef CURSORS_HPP
#define CURSORS_HPP

#include <windows.h>

enum Cursor {
	crNormal,
	crIgnore,
	crMove,
	crResizeNS,
	crResizeWE,
	crResizeNWSE,
	crResizeNESW,
	crLast // must be largest enum value
};

/* A store for the mouse cursors we use.
 * They are loaded with the LR_SHARED flag, so they should not be destroyed.
 */
class Cursors {

	HCURSOR d_cursors[crLast];

	public:

		Cursors();

		HCURSOR get(Cursor cursor) const;

	private:

		/* Loads the system cursor with the specified OCR_* constant.
		 */
		HCURSOR load(int ocr) const;
};

#endif
