#ifndef MAIN_HPP
#define MAIN_HPP

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

/* Apply the given configuration to make it active.
 */
void applyConfig(Configuration const &config);

#endif
