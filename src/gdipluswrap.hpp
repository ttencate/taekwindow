#ifndef GDIPLUSWRAP_HPP
#define GDIPLUSWRAP_HPP

#include <windows.h>
#include <gdiplus.h>

/* A wrapper for a GDI+ token.
 */
class GdiPlusWrap {

	/* GDI+ token.
	 */
	ULONG_PTR d_token;

	bool d_ok;

	public:

		GdiPlusWrap();
		~GdiPlusWrap();

};

#endif
