#include <tchar.h>
#include <shlobj.h>

#include "shelllink.hpp"
#include "errors.hpp"

ShellLink::ShellLink(const TCHAR *filename)
:
	d_filename(filename)
{
}

bool ShellLink::exists() const {
	return GetFileAttributes(d_filename) != INVALID_FILE_ATTRIBUTES;
}

/* Uses the COM interface IShellLink to create a shortcut.
 */
void ShellLink::create(TCHAR *target, TCHAR *workingDir, TCHAR *description) {
	HRESULT res;

	// Initialize COM.
	res = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	if (res != S_OK && res != S_FALSE) {
		showError(NULL, _T("Error creating shortcut"), _T("COM initialization failed. Error code: %1!lx!."), res);
		return;
	}

	// Create the ShellLink object.
	IShellLink *link;
	res = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&link);
	if (res != S_OK) {
		showError(NULL, _T("Error creating shortcut"), _T("The ShellLink object could not be created. Error code: %1!lx!."), res);
	} else {
		// Set the link's properties.
		link->SetPath(target);
		link->SetDescription(description);
		link->SetWorkingDirectory(workingDir);
		link->SetIconLocation(target, 0);

		// Save the link to the file.
		IPersistFile *pf;
		link->QueryInterface(IID_IPersistFile, (void**)&pf);
		if (res != S_OK) {
			showError(NULL, _T("Error creating shortcut"), _T("The IPersistFile interface could not be acquired. Error code: %1!lx!."), res);
		} else {
			res = pf->Save(d_filename, TRUE);
			if (res != S_OK) {
				showError(NULL, _T("Error creating shortcut"), _T("The ShellLink could not be saved. Error code: %1!lx!."), res);
			}
			// Clean up.
			pf->Release();
		}
		link->Release();
	}
}

void ShellLink::destroy() {
	DeleteFile(d_filename);
}
