#ifndef _DEBUGLOG_HPP_
#define _DEBUGLOG_HPP_

#if _DEBUG

void openDebugLog();
void closeDebugLog();
void debugLog(char const *format, ...);

#define OPENDEBUGLOG openDebugLog
#define CLOSEDEBUGLOG closeDebugLog
#define DEBUGLOG debugLog

#else /* !_DEBUG */

#define OPENDEBUGLOG()
#define CLOSEDEBUGLOG()
#define DEBUGLOG(format, ...)

#endif /* _DEBUG */

#endif
