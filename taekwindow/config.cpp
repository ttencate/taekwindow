#include "config.hpp"
#include "util.hpp"
#include "version.h"

#include <windows.h>
#include <shlobj.h>
#include <tchar.h>
#include <strsafe.h>

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

/* Reads a DWORD from the specified value in the specified registry key,
 * and converts it to some compatible type.
 * If the value exists and has the DWORD type, it is written to the variable pointed to by out and true is returned.
 * Otherwise, nothing happens and false is returned.
 */
template<typename T>
bool readDWord(HKEY key, LPCWSTR valueName, T &out) {
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

/* Reads the configuration settings from the registry and places them in the objects pointed to.
 */
void loadConfigFromRegistry(DLLConfiguration *dllConfig, EXEConfiguration *exeConfig) {
	// Open the registry keys.
	HKEY softwareKey;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, L"Software", 0, KEY_READ, &softwareKey) == ERROR_SUCCESS) {
		HKEY appKey;
		if (RegOpenKeyEx(softwareKey, L"Taekwindow", 0, KEY_READ, &appKey) == ERROR_SUCCESS) {
			HKEY versionKey;
			// We'll only change the version number of the key once the registry structure is no longer backwards compatible.
			// That is, once newer versions can no longer interpret the settings of an older version as if the settings were their own.
			if (RegOpenKeyEx(appKey, L"0.2", 0, KEY_READ, &versionKey) == ERROR_SUCCESS) {
				// Read stuff for the DLL.
				int x = sizeof(DWORD);
				int y = sizeof(int);
				readDWord(versionKey, L"modifier"               , dllConfig->modifier      );
				readDWord(versionKey, L"moveButton"             , dllConfig->moveButton    );
				readDWord(versionKey, L"resizeButton"           , dllConfig->resizeButton  );
				readDWord(versionKey, L"resizeMode"             , dllConfig->resizeMode    );
				readDWord(versionKey, L"pushBackButton"         , dllConfig->pushBackButton);
				readDWord(versionKey, L"scrollWindowUnderCursor", dllConfig->scrollWindowUnderCursor);
				// Read stuff for the EXE.
				readDWord(versionKey, L"systemTrayIcon", exeConfig->systemTrayIcon);
				// Close the keys again.
				RegCloseKey(versionKey);
			}
			RegCloseKey(appKey);
		}
		RegCloseKey(softwareKey);
	}
}

/* Saves the given configuration to the registry.
 */
void saveConfigToRegistry(DLLConfiguration *dllConfig, EXEConfiguration *exeConfig) {
	// TODO
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
	loadConfigFromRegistry(dllConfig, exeConfig);
	loadConfigFromStartup(exeConfig);
}

void saveConfig(DLLConfiguration *dllConfig, EXEConfiguration *exeConfig) {
	saveConfigToRegistry(dllConfig, exeConfig);
	saveConfigToStartup(exeConfig);
}
