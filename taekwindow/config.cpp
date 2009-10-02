#include "config.hpp"
#include "util.hpp"
#include "version.h"

#include <windows.h>
#include <shlobj.h>
#include <tchar.h>
#include <strsafe.h>

/* We'll only change the version number of the key once the registry structure is no longer backwards compatible.
 * That is, once newer versions can no longer interpret the settings of an older version as if the settings were their own.
 */
TCHAR const REG_KEY[] = _T("Software\\Taekwindow\\0.2");

void EXEConfiguration::setDefaults() {
	systemTrayIcon = true;
	startAtLogon = false;
}

/* Constructs the filename of the shortcut link to the EXE in the Startup folder in the Start Menu.
 * The buffer must be at least MAX_PATH characters long.
 */
void getStartupLinkFilename(TCHAR *buffer) {
	SHGetFolderPath(0, CSIDL_STARTUP, NULL, SHGFP_TYPE_CURRENT, buffer);
	StringCchCat(buffer, MAX_PATH, _T("\\Taekwindow.lnk"));
}

/* Some template trickery for type safety when loading stuff from DWORD values.
 */
template<typename T> void assign(T &out, DWORD data) { out = T(data); }
template<> void assign<bool>(bool &out, DWORD data) { out = (data != 0); }

/* Uses the COM interface IShellLink to create a shortcut.
 */
void createLink(TCHAR *filename, TCHAR *target, TCHAR *workingDir, TCHAR *description) {
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
			res = pf->Save(filename, TRUE);
			if (res != S_OK) {
				showError(NULL, _T("Error creating shortcut"), _T("The ShellLink could not be saved. Error code: %1!lx!."), res);
			}
			// Clean up.
			pf->Release();
		}
		link->Release();
	}
}

struct Read {
	/* Reads a DWORD from the specified value in the specified registry key,
	 * and converts it to some compatible type.
	 * If the value exists and has the DWORD type, it is written to the variable pointed to by out and true is returned.
	 * Otherwise, nothing happens and false is returned.
	 */
	template<typename T>
	static bool apply(T &out, TCHAR const *valueName, HKEY key) {
		DWORD type;
		DWORD data;
		DWORD size = sizeof(data);
		if (RegQueryValueEx(key, valueName, NULL, &type, (LPBYTE)&data, &size) == ERROR_SUCCESS) {
			if (type == REG_DWORD && size == sizeof(data)) {
				assign(out, data);
				return true;
			}
		}
		return false;
	}
};

struct Write {
	/* Writes a DWORD to the specified value in the specified registry key,
	 * converting it from its original type.
	 * If writing was successful, true is returned.
	 */
	template<typename T>
	static bool apply(T &in, TCHAR const *valueName, HKEY key) {
		DWORD data = (DWORD)in;
		return RegSetValueEx(key, valueName, 0, REG_DWORD, (const BYTE*)&data, sizeof(data)) == ERROR_SUCCESS;
	}
};

/* Applies the function F::apply() to all configuration items that are stored in the registry.
 * F::apply<T>(T&, TCHAR const*, P)
 * where T is the type of the configuration item, and P the type of some parameter.
 */
template<typename F, typename P>
void applyFunctor(DLLConfiguration *dllConfig, EXEConfiguration *exeConfig, P param) {
	F::apply(dllConfig->modifier, _T("modifier"), param);
	F::apply(dllConfig->moveButton, _T("moveButton"), param);
	F::apply(dllConfig->resizeButton, _T("resizeButton"), param);
	F::apply(dllConfig->resizeMode, _T("resizeMode"), param);
	F::apply(dllConfig->pushBackButton, _T("pushBackButton"), param);
	F::apply(dllConfig->scrollWindowUnderCursor, _T("scrollWindowUnderCursor"), param);

	F::apply(exeConfig->systemTrayIcon, _T("systemTrayIcon"), param);
}

/* Reads the configuration settings from the registry and places them in the objects pointed to.
 */
void loadConfigFromRegistry(DLLConfiguration *dllConfig, EXEConfiguration *exeConfig) {
	HKEY key;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, REG_KEY, 0, KEY_READ, &key) == ERROR_SUCCESS) {
		applyFunctor<Read>(dllConfig, exeConfig, key);
		RegCloseKey(key);
	}
}

/* Saves the given configuration to the registry.
 */
void saveConfigToRegistry(DLLConfiguration *dllConfig, EXEConfiguration *exeConfig) {
	// Open the registry key.
	HKEY key;
	// We'll only change the version number of the key once the registry structure is no longer backwards compatible.
	// That is, once newer versions can no longer interpret the settings of an older version as if the settings were their own.
	if (RegOpenKeyEx(HKEY_CURRENT_USER, REG_KEY, 0, KEY_SET_VALUE, &key) == ERROR_SUCCESS) {
		applyFunctor<Write>(dllConfig, exeConfig, key);
		// Close the key again.
		RegCloseKey(key);
	}
}

/* Checks the existence of a Startup shortcut and places it in execonfig.
 */
void loadConfigFromStartup(EXEConfiguration *exeConfig) {
	TCHAR linkFilename[MAX_PATH];
	getStartupLinkFilename(linkFilename);
	exeConfig->startAtLogon = (GetFileAttributes(linkFilename) != INVALID_FILE_ATTRIBUTES);
}

/* Creates or removes a Startup shortcut as necessary.
 */
void saveConfigToStartup(EXEConfiguration *exeConfig) {
	TCHAR linkFilename[MAX_PATH];
	getStartupLinkFilename(linkFilename);
	bool exists = GetFileAttributes(linkFilename) != INVALID_FILE_ATTRIBUTES;
	if (exists && !exeConfig->startAtLogon) {
		DeleteFile(linkFilename);
	} else if (!exists && exeConfig->startAtLogon) {
		TCHAR target[MAX_PATH];
		GetModuleFileName(NULL, target, MAX_PATH);
		TCHAR workingDir[MAX_PATH];
		TCHAR *filePart;
		GetFullPathName(target, MAX_PATH, workingDir, &filePart);
		if (filePart) {
			*filePart = '\0';
		}
		createLink(linkFilename, target, workingDir, _T(APPLICATION_TITLE) _T(" startup link (automatically created; can safely be removed)"));
	}
}

void loadConfig(DLLConfiguration *dllConfig, EXEConfiguration *exeConfig) {
	// Set the defaults, in case settings are missing.
	dllConfig->setDefaults();
	exeConfig->setDefaults();
	loadConfigFromRegistry(dllConfig, exeConfig);
	loadConfigFromStartup(exeConfig);
}

void saveConfig(DLLConfiguration *dllConfig, EXEConfiguration *exeConfig) {
	saveConfigToRegistry(dllConfig, exeConfig);
	saveConfigToStartup(exeConfig);
}
