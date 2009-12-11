#ifndef _MAIN_HPP_
#define _MAIN_HPP_

#include <windows.h>

#include "config.hpp"

/* Whether or not the event hooks are currently attached.
 * I.e.: whether the program is currently "enabled".
 */
bool isEnabled();

/* Enables or disables the program.
 * Returns true on success.
 */
bool enable();
bool disable();

/* Returns the handle of the current instance, as passed to WinMain.
 */
HINSTANCE getCurrentInstance();

/* The currently active configuration.
 */
extern Configuration activeConfig;

/* Apply the given configuration to make it active.
 */
void applyConfig(Configuration *config);

#endif
