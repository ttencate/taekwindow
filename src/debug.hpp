#ifndef DEBUGLOG_HPP
#define DEBUGLOG_HPP

#ifdef _DEBUG

#include <windows.h>
#include <tchar.h>

void openDebugLog();
void closeDebugLog();
void debugLog(LPCTSTR format, ...);
void assert(bool x, TCHAR const *file, size_t line);

#define OPENDEBUGLOG() openDebugLog()
#define CLOSEDEBUGLOG() closeDebugLog()
#define DEBUGLOG(format, ...) debugLog(_T(format), __VA_ARGS__)
#define CONDDEBUGLOG(condition, ...) if (condition) DEBUGLOG(__VA_ARGS__)

#define ASSERT(x) assert((x) != 0, _T(__FILE__), __LINE__)

#else // !_DEBUG

#define OPENDEBUGLOG()
#define CLOSEDEBUGLOG()
#define DEBUGLOG(...)
#define CONDDEBUGLOG(condition, ...)
#define ASSERT(x)

#endif // _DEBUG

#endif
