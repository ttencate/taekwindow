#ifndef _DLLCONFIG_HPP_
#define _DLLCONFIG_HPP_

#include "types.hpp"

class DLLConfiguration {

	public:

		/* The modifier key used for moving and resizing.
		 * This is referred to as Modifier is the comments, but you can read Alt if you like.
		 */
		int modifier;

		/* The mouse buttons used for moving and resizing, respectively.
		 */
		MouseButton moveButton, resizeButton;

		/* The behaviour to use when resizing windows.
		 */
		ResizeMode resizeMode;
};

#endif
