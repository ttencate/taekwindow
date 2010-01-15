#ifndef TYPES_HPP
#define TYPES_HPP

#include <windows.h>

/* rmBottomRight:   always resize from the bottom right corner of the window, regardless of where it was grabbed.
 * rmClosestCorner: divide window into 9 equally sized rectangles and pick the appropriate corner/edge when resizing.
 */
enum ResizeMode { rmBottomRight = 0, rmNineRectangles = 1 };

/* Represents a button on the mouse.
 */
enum MouseButton { mbLeft = 0, mbMiddle = 1, mbRight = 2, mbNone = 0x7FFF };

/* For a given mouse event (e.g. WM_LBUTTONDOWN), returns the corresponding button from our enum.
 */
MouseButton eventToButton(WPARAM wParam);

/* Returns WM_LBUTTONDOWN for mbLeft, etc.
 */
UINT buttonDownMessage(MouseButton button);

/* Locations where settings can be stored.
 */
enum ConfigurationLocation { clRegistry = 0, clIniFile = 1 };

#endif