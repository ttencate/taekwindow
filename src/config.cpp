#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <shlobj.h>

#include "config.hpp"
#include "errors.hpp"
#include "version.hpp"
#include "shelllink.hpp"

/* We'll only change the version number of the key once the registry structure is no longer backwards compatible.
 * That is, once newer versions can no longer interpret the settings of an older version as if the settings were their own.
 */
TCHAR const *const Configuration::REG_KEY = _T("Software\\Taekwindow\\0.2");

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
void Configuration::applyFunctor(P param) {
	F::apply(modifier, _T("modifier"), param);
	F::apply(moveButton, _T("moveButton"), param);
	F::apply(resizeButton, _T("resizeButton"), param);
	F::apply(resizeMode, _T("resizeMode"), param);
	F::apply(pushBackButton, _T("pushBackButton"), param);
	F::apply(scrollWindowUnderCursor, _T("scrollWindowUnderCursor"), param);

	F::apply(systemTrayIcon, _T("systemTrayIcon"), param);
}

/* Reads the configuration settings from the registry and places them in the objects pointed to.
 */
void Configuration::loadFromRegistry() {
	HKEY key;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, REG_KEY, 0, KEY_READ, &key) == ERROR_SUCCESS) {
		applyFunctor<Read>(key);
		RegCloseKey(key);
	}
}

/* Saves the given configuration to the registry.
 */
void Configuration::saveToRegistry() {
	// Open the registry key.
	HKEY key;
	// We'll only change the version number of the key once the registry structure is no longer backwards compatible.
	// That is, once newer versions can no longer interpret the settings of an older version as if the settings were their own.
	if (RegOpenKeyEx(HKEY_CURRENT_USER, REG_KEY, 0, KEY_SET_VALUE, &key) == ERROR_SUCCESS) {
		applyFunctor<Write>(key);
		// Close the key again.
		RegCloseKey(key);
	}
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
	loadFromRegistry();
	loadFromStartup();
}

void Configuration::save() {
	saveToRegistry();
	saveToStartup();
}
