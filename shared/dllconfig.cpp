#include "dllconfig.hpp"

#include <windows.h>

void DLLConfiguration::setDefaults() {
	modifier = VK_LMENU;
	moveButton = mbLeft;
	resizeButton = mbRight;
	resizeMode = rmNineRectangles;
	pushBackButton = mbMiddle;
	scrollWindowUnderCursor = true;
}
