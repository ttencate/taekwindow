#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "comctl.hpp"
#include "util.hpp"

CommonControls::CommonControls() {
	// Huhhuh, init common control sex. Allcaps break my head.
	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icc.dwICC = ICC_STANDARD_CLASSES | ICC_LINK_CLASS;
	if (InitCommonControlsEx(&icc) != TRUE) {
		showLastError(NULL, _T("Unable to initialize common controls"));
		// TODO should be fatal error
	}
}
