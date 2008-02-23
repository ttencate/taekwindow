#ifndef _MAIN_HPP_
#define _MAIN_HPP_

/* Whether or not the event hooks are currently attached.
 * I.e.: whether the program is currently "enabled".
 */
bool isEnabled();

/* Enables or disables the program.
 * Returns true on success.
 */
bool enable();
bool disable();

#endif
