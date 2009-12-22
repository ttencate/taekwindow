#ifndef PROPSHEET_HPP
#define PROPSHEET_HPP

#include <windows.h>

#include "propsheetpage.hpp"

/* Wrapper for a PropertySheet.
 *
 * Important: due to the way the callback is implemented,
 * only one property sheet must be shown through this class at the same time!
 * This can be hacked around by using GetProp/SetProp, but it's not worth the trouble right now.
 */
class PropSheet {

	static size_t const s_maxNumPages = 10;
	static PropSheet *s_instance;

	PROPSHEETHEADER d_header;
	PROPSHEETPAGE d_pages[s_maxNumPages];

	public:

		PropSheet(TCHAR const *caption, HICON icon);

		void addPage(PropSheetPage const &page);

		INT_PTR showModal();

	private:

		static int CALLBACK propSheetProc(HWND hwnd, UINT msg, LPARAM lParam);
		virtual void callback(HWND hwnd, UINT msg, LPARAM lParam);

};

#endif
