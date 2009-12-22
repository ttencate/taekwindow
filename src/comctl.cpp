#include <tchar.h>

#include "comctl.hpp"
#include "errors.hpp"

CommonControls::CommonControls() {
	// Initialize the sex of a common control. Or something.
	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icc.dwICC = ICC_STANDARD_CLASSES | ICC_LINK_CLASS;
	if (InitCommonControlsEx(&icc) != TRUE) {
		showLastError(NULL, _T("Unable to initialize common controls"));
		// TODO should be fatal error
	}
}
