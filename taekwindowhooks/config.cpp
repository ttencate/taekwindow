#include "config.hpp"

#include <windows.h>

void setConfigDefaults(DLLConfiguration *config) {
	config->modifier = VK_LMENU;
	config->moveButton = mbLeft;
	config->resizeButton = mbRight;
	config->resizeMode = rmNineRectangles;
}
