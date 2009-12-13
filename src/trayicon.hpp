#ifndef TRAYICON_HPP
#define TRAYICON_HPP

#include <windows.h>

/* Sets the visibility of the tray icon.
 * It is okay to call this if the visibility state does not change
 * (e.g. showing it when it is already visible).
 * In case of errors, the function fails silently.
 */
void showTrayIcon(bool show);

/* Returns whether the tray icon is currently visible.
 */
bool showingTrayIcon();

/* Updates the tray icon (if visible) with the current status.
 */
void updateTrayIcon();

#endif
