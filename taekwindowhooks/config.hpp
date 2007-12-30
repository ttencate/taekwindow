#ifndef _CONFIG_HPP_
#define _CONFIG_HPP_

#include <windows.h>

/* The modifier key used for moving and resizing.
 * This is referred to as Modifier is the comments, but you can read Alt if you like.
 */
extern int modifier;

/* Reads the configuration from the registry.
 * If no value is present for a certain setting, that setting remains untouched.
 */
void __declspec(dllexport) __stdcall readConfig();

#endif
