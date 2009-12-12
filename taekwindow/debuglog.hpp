#ifndef _DEBUGLOG_HPP_
#define _DEBUGLOG_HPP_

#ifdef _DEBUG

#include <windows.h>
#include <tchar.h>

void openDebugLog();
void closeDebugLog();
void debugLog(LPCTSTR format, ...);

// Note that '...' in a macro must always represent at least one argument;
// we therefore leave the 'format' argument of debugLog off.
#define OPENDEBUGLOG() openDebugLog()
#define CLOSEDEBUGLOG() closeDebugLog()
#define DEBUGLOG(format, ...) debugLog(_T(format), __VA_ARGS__)
#define CONDDEBUGLOG(condition, ...) if (condition) DEBUGLOG(__VA_ARGS__)

#else // !_DEBUG

#define OPENDEBUGLOG()
#define CLOSEDEBUGLOG()
#define DEBUGLOG(...)
#define CONDDEBUGLOG(condition, ...)

#endif // _DEBUG

#endif
