#ifndef HACKS_HPP
#define HACKS_HPP

#include <windows.h>

bool isMIRCWindow(HWND window);
HWND findMIRCTextWindow(HWND containerWindow);
bool isGoogleTalk(HWND window);
bool isGoogleChrome(HWND window);
bool isItunes(HWND window);
bool isMSOfficeDocument(HWND window);

#endif
