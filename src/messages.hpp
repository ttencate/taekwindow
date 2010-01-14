#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include <windows.h>

#define TRAYICON_MESSAGE    (WM_APP + 1)
#define SHOW_CONFIG_MESSAGE (WM_APP + 2)
#define DRAG_START_MESSAGE  (WM_APP + 3) // wParam: MouseButton; lParam: mousePos
#define DRAG_END_MESSAGE    (WM_APP + 4) // wParam: MouseButton; lParam: mousePos
#define DRAG_MOVE_MESSAGE   (WM_APP + 5) // wParam: unused; lParam: mousePos

#endif
