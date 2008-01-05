#include "config.hpp"

/* Reads a DWORD from the specified value in the specified registry key.
 * If the value exists and has the DWORD type, it is written to the variable pointed to by out and true is returned.
 * Otherwise, nothing happens and false is returned.
 */
bool readDWord(HKEY key, LPCSTR valueName, LPDWORD out) {
	DWORD type;
	DWORD data;
	DWORD size = sizeof(data);
	if (RegQueryValueEx(key, valueName, NULL, &type, (LPBYTE)&data, &size) == ERROR_SUCCESS) {
		if (type == REG_DWORD && size == sizeof(data)) {
			*out = data;
			return true;
		}
	}
	return false;
}

void readConfig() {
	// Open the registry keys.
	HKEY software;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software", 0, KEY_READ, &software) == ERROR_SUCCESS) {
		HKEY taekwindow;
		if (RegOpenKeyEx(software, "Taekwindow", 0, KEY_READ, &taekwindow) == ERROR_SUCCESS) {
			HKEY ohPointTwo;
			// We'll only change the version number of the key once the registry structure is no longer backwards compatible.
			// That is, once newer versions can no longer interpret the settings of an older version as if the settings were their own.
			if (RegOpenKeyEx(taekwindow, "0.2", 0, KEY_READ, &ohPointTwo) == ERROR_SUCCESS) {
				// Read stuff.
				readDWord(ohPointTwo, "modifier"    , (LPDWORD)&modifier    );
				readDWord(ohPointTwo, "moveButton"  , (LPDWORD)&moveButton  );
				readDWord(ohPointTwo, "resizeButton", (LPDWORD)&resizeButton);
				readDWord(ohPointTwo, "resizeMode"  , (LPDWORD)&resizeMode  );
				// Close the keys again.
				RegCloseKey(ohPointTwo);
			}
			RegCloseKey(taekwindow);
		}
		RegCloseKey(software);
	}
}

