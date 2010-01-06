#define STRSAFE_NO_DEPRECATE 1

#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <shlobj.h>

#include "config.hpp"
#include "errors.hpp"
#include "version.h"
#include "shelllink.hpp"

/* We'll only change the version number of the key once the registry structure is no longer backwards compatible.
 * That is, once newer versions can no longer interpret the settings of an older version as if the settings were their own.
 */
#define BASE_KEY _T("Software")
#define PROGRAM_KEY _T("Taekwindow")
#define VERSION_KEY _T("0.2")

TCHAR const *const Configuration::s_baseKey = BASE_KEY;
TCHAR const *const Configuration::s_programKey = PROGRAM_KEY;
TCHAR const *const Configuration::s_versionKey = VERSION_KEY;

TCHAR const *const Configuration::s_regKey = BASE_KEY _T("\\") PROGRAM_KEY _T("\\") VERSION_KEY;

Configuration::Configuration()
:
	systemTrayIcon(true),
	startAtLogon(false),
	modifier(VK_LMENU),
	moveButton(mbLeft),
	resizeButton(mbRight),
	resizeMode(rmNineRectangles),
	pushBackButton(mbMiddle),
	scrollWindowUnderCursor(true)
{
}

void Configuration::setDefaults() {
	*this = Configuration();
}

/* Constructs the filename of the shortcut link to the EXE in the Startup folder in the Start Menu.
 * The buffer must be at least MAX_PATH characters long.
 */
void Configuration::getStartupLinkFilename(TCHAR *buffer) {
	SHGetFolderPath(0, CSIDL_STARTUP, NULL, SHGFP_TYPE_CURRENT, buffer);
	StringCchCat(buffer, MAX_PATH, _T("\\Taekwindow.lnk"));
}

/* Some template trickery for type safety when loading stuff from DWORD values.
 */
template<typename T> void assign(T &out, DWORD data) { out = T(data); }
template<> void assign<bool>(bool &out, DWORD data) { out = (data != 0); }

struct Configuration::ReadFromRegistry {
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

struct Configuration::WriteToRegistry {
	/* Writes a DWORD to the specified value in the specified registry key,
	 * converting it from its original type.
	 * If writing was successful, true is returned.
	 */
	template<typename T>
	static bool apply(T const &in, TCHAR const *valueName, HKEY key) {
		DWORD data = (DWORD)in;
		return RegSetValueEx(key, valueName, 0, REG_DWORD, (const BYTE*)&data, sizeof(data)) == ERROR_SUCCESS;
	}
};

struct Configuration::ReadFromIni {
	/* Reads a UINT from the specified key in the INI file.
	 */
	template<typename T>
	static bool apply(T &out, TCHAR const *keyName, TCHAR const *filename) {
		UINT data = GetPrivateProfileInt(s_versionKey, keyName, out, filename);
		assign(out, data);
		return true;
	}
};

struct Configuration::WriteToIni {
	/* Writes an INT to the specified key in the INI file.
	 */
	template<typename T>
	static bool apply(T const &in, TCHAR const *keyName, TCHAR const *filename) {
		TCHAR buffer[20];
		int inInt = in;
		wsprintf(buffer, _T("%i"), inInt);
		WritePrivateProfileString(s_versionKey, keyName, buffer, filename);
		return true;
	}
};

/* Applies the function F::apply() to all configuration items that are stored in the registry.
 * F::apply<T>(T&, TCHAR const*, P)
 * where T is the type of the configuration item, and P the type of some parameter.
 */
template<typename F, typename P>
void Configuration::applyFunctor(P param) {
	F::apply(systemTrayIcon, _T("systemTrayIcon"), param);
	F::apply(modifier, _T("modifier"), param);
	F::apply(moveButton, _T("moveButton"), param);
	F::apply(resizeButton, _T("resizeButton"), param);
	F::apply(resizeMode, _T("resizeMode"), param);
	F::apply(pushBackButton, _T("pushBackButton"), param);
	F::apply(scrollWindowUnderCursor, _T("scrollWindowUnderCursor"), param);
}

/* Reads the configuration settings from the registry and places them in this object.
 */
void Configuration::loadFromRegistry() {
	HKEY key;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, s_regKey, 0, KEY_READ, &key) == ERROR_SUCCESS) {
		applyFunctor<ReadFromRegistry>(key);
		RegCloseKey(key);
	}
}

/* Saves the given configuration to the registry.
 */
void Configuration::saveToRegistry() {
	HKEY key;
	LONG error = RegCreateKeyEx(HKEY_CURRENT_USER, s_regKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &key, NULL);
	if (error != ERROR_SUCCESS) {
		showSystemError(NULL, error, _T("Failed to save settings to registry"));
		return;
	}
	applyFunctor<WriteToRegistry>(key);
	RegCloseKey(key);
}

/* Deletes all settings from the registry.
 */
void Configuration::deleteFromRegistry() {
	HKEY baseKey;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, s_baseKey, 0, KEY_READ, &baseKey) == ERROR_SUCCESS) {
		HKEY programKey;
		if (RegOpenKeyEx(baseKey, s_programKey, 0, KEY_READ, &programKey) == ERROR_SUCCESS) {
			RegDeleteKey(programKey, s_versionKey);
		}
		RegDeleteKey(baseKey, s_programKey);
	}
}

bool Configuration::existsIni() {
	TCHAR filename[MAX_PATH];
	getIniFilename(filename, MAX_PATH);
	return GetFileAttributes(filename) != INVALID_FILE_ATTRIBUTES;
}

void Configuration::getIniFilename(TCHAR *buffer, size_t size) {
	DWORD length = GetModuleFileName(NULL, buffer, size);
	if (length < 3) {
		buffer[0] = '\0';
		return;
	}
	StringCchCopy(&buffer[length - 3], 4, _T("ini"));
}

void Configuration::loadFromIni() {
	TCHAR filename[MAX_PATH];
	getIniFilename(filename, MAX_PATH);
	applyFunctor<ReadFromIni>(filename);
}

void Configuration::saveToIni() {
	TCHAR filename[MAX_PATH];
	getIniFilename(filename, MAX_PATH);
	applyFunctor<WriteToIni>(filename);
}

void Configuration::deleteFromIni() {
	TCHAR filename[MAX_PATH];
	getIniFilename(filename, MAX_PATH);
	DeleteFile(filename);
}

/* Checks the existence of a Startup shortcut and places it in execonfig.
 */
void Configuration::loadFromStartup() {
	TCHAR linkFilename[MAX_PATH];
	getStartupLinkFilename(linkFilename);
	startAtLogon = (GetFileAttributes(linkFilename) != INVALID_FILE_ATTRIBUTES);
}

/* Creates or removes a Startup shortcut as necessary.
 */
void Configuration::saveToStartup() {
	TCHAR linkFilename[MAX_PATH];
	getStartupLinkFilename(linkFilename);
	ShellLink link(linkFilename);

	bool exists = link.exists();
	if (exists && !startAtLogon) {
		link.destroy();
	} else if (!exists && startAtLogon) {
		TCHAR target[MAX_PATH];
		GetModuleFileName(NULL, target, MAX_PATH);
		TCHAR workingDir[MAX_PATH];
		TCHAR *filePart;
		GetFullPathName(target, MAX_PATH, workingDir, &filePart);
		if (filePart) {
			*filePart = '\0';
		}
		link.create(target, workingDir, _T(APPLICATION_TITLE) _T(" startup link (automatically created; can safely be removed)"));
	}
}

void Configuration::load() {
	// Set the defaults, in case settings are missing.
	setDefaults();

	if (existsIni()) {
		location = clIniFile;
		loadFromIni();
	} else {
		location = clRegistry;
		loadFromRegistry();
	}

	loadFromStartup();
}

void Configuration::save() {
	switch (location) {
		case clIniFile:
			deleteFromRegistry();
			saveToIni();
			break;
		case clRegistry:
			deleteFromIni();
			saveToRegistry();
			break;
	}
	
	saveToStartup();
}
