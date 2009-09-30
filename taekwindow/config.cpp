#include "config.hpp"

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
	// TODO
}

void loadConfig(DLLConfiguration *dllConfig, EXEConfiguration *exeConfig) {
	loadConfigFromRegistry(dllConfig, exeConfig);
	loadConfigFromStartup(exeConfig);
}

void saveConfig(DLLConfiguration *dllConfig, EXEConfiguration *exeConfig) {
	saveConfigToRegistry(dllConfig, exeConfig);
	saveConfigToStartup(exeConfig);
}
