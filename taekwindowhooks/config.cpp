#include "config.hpp"

void __declspec(dllexport) __stdcall readConfig() {
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
				DWORD type;
				DWORD data;
				DWORD size = sizeof(data);
				if (RegQueryValueEx(ohPointTwo, "modifier", NULL, &type, (LPBYTE)&data, &size) == ERROR_SUCCESS) {
					if (type == REG_DWORD && size == sizeof(data)) {
						modifier = data;
					}
				}
				// Close the keys again.
				RegCloseKey(ohPointTwo);
			}
			RegCloseKey(taekwindow);
		}
		RegCloseKey(software);
	}
}

