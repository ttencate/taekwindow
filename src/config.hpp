#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "types.hpp"

struct Configuration {

public:

	/* Whether the settings are stored in the registry or in an INI file.
	 */
	ConfigurationLocation location;

	/* Whether or not to show the system tray icon while the application is running.
	 */
	bool systemTrayIcon;

	/* Whether or not to launch at logon.
	 * Implemented by a shortcut in the Startup folder in the Start Menu.
	 */
	bool startAtLogon;

	/* The modifier key used for moving and resizing, as a virtual-key code.
	 * This is referred to as Modifier in the comments, but you can read Alt if you like.
	 */
	int modifier;

	/* The mouse buttons used for moving and resizing, respectively.
	 */
	MouseButton moveButton, resizeButton;

	/* The behaviour to use when resizing windows.
	 */
	ResizeMode resizeMode;

	/* The mouse button that, when clicked on the title bar, pushes the window to the back.
	 */
	MouseButton pushBackButton;

	/* The mouse wheel behaviour: scroll the window under the cursor (true), or the focused window (false)?
	 */
	bool scrollWindowUnderCursor;

	/* Sets all fields to their default values.
	 */
	Configuration();
	
	/* Sets all fields to their default values.
	 */
	void setDefaults();

	/* Loads the configuration from the registry/environment.
	 * In case of missing values, fills in defaults.
	 */
	void load();

	/* Saves the configuration to the registry/environment.
	 */
	void save();

private:

	static TCHAR const *const s_baseKey;
	static TCHAR const *const s_programKey;
	static TCHAR const *const s_versionKey;
	static TCHAR const *const s_regKey;

	struct ReadFromRegistry;
	struct WriteToRegistry;
	struct ReadFromIni;
	struct WriteToIni;

	void getStartupLinkFilename(TCHAR *buffer);
	template<typename F, typename P> void applyFunctor(P param);
	
	void loadFromRegistry();
	void saveToRegistry();
	void deleteFromRegistry();

	bool existsIni();
	void getIniFilename(TCHAR *buffer, size_t size);
	void loadFromIni();
	void saveToIni();
	void deleteFromIni();

	void loadFromStartup();
	void saveToStartup();

};

#endif
