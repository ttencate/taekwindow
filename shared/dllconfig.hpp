#ifndef _DLLCONFIG_HPP_
#define _DLLCONFIG_HPP_

#include "types.hpp"

struct DLLConfiguration {

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
	void setDefaults();

};

#endif
