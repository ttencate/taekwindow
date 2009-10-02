#ifndef _CONFIGDLG_HPP_
#define _CONFIGDLG_HPP_

/* Unconditionally shows the configuration dialog.
 * Must not be called when it is already showing.
 */
void showConfig();

/* Returns true if the configuration dialog is currently visible.
 */
bool isConfigShowing();

/* Brings the configuration dialog to the foreground and gives it focus,
 * if it is currently showing.
 */
void focusConfig();

#endif
