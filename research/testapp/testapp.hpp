#ifndef _TESTAPP_HPP_
#define _TESTAPP_HPP_

#include <windows.h>

// messages.cpp
char *messageName(UINT message);
char *messageInfo(MSG *message);
char *messageInfo(UINT uMsg, WPARAM wParam, LPARAM lParam);
char *retValInfo(LRESULT retval, UINT message);

// logging.cpp
void startLogging();
void endLogging();
void indent();
void outdent();
void log(const char *format, ...);

#endif
