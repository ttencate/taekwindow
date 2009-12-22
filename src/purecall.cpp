#include <tchar.h>

#include "errors.hpp"
#include "version.hpp"

extern "C" int __cdecl _purecall() {
	showError(NULL, _T(APPLICATION_TITLE) _T(": fatal error"), _T("Pure virtual function called. Aborting."));
	abort();
}
