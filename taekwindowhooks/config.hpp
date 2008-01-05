#ifndef _CONFIG_HPP_
#define _CONFIG_HPP_

#include <windows.h>

#include "util.hpp"

// Note: actual configuration variables are declared in the shared data segment in shared.cpp, not in config.cpp.

/* The modifier key used for moving and resizing.
 * This is referred to as Modifier is the comments, but you can read Alt if you like.
 */
extern int modifier;

/* The mouse buttons used for moving and resizing, respectively.
 */
extern MouseButton moveButton, resizeButton;

/* The behaviour to use when resizing windows.
 */
extern ResizeMode resizeMode;

/* Reads the configuration from the registry.
 * If no value is present for a certain setting, that setting remains untouched.
 */
void readConfig();

#endif
