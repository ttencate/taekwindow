#include "config.hpp"

#include <windows.h>

void setDLLConfigDefaults(DLLConfiguration *config) {
	config->modifier = VK_LMENU;
	config->moveButton = mbLeft;
	config->resizeButton = mbRight;
	config->resizeMode = rmNineRectangles;
	config->pushBackButton = mbMiddle;
}
