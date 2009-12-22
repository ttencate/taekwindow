#ifndef COMCTL_HPP
#define COMCTL_HPP

#include <windows.h>
#include <commctrl.h>

/* Initializes the Common Controls library.
 * Any class using the common controls should ensure that an object of this class
 * exists during its lifetime.
 */
class CommonControls {
	public:
		CommonControls();
};

#endif