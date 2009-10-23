#ifdef _DEBUG

#include <windows.h>
#include <tchar.h>
#include <strsafe.h>

#include "debuglog.hpp"
#include "util.hpp"

extern HANDLE debugLogFile;
HANDLE localDebugLogFile = INVALID_HANDLE_VALUE;

/* Steal this from main.cpp.
 */
extern DWORD mainProcessId;

void openDebugLog() {
	debugLogFile = INVALID_HANDLE_VALUE;
	long time = GetTickCount();
	TCHAR fileName[64];
	wsprintf(fileName, _T("taekwindow-debug-%d.%03d.log"), time/1000, time%1000);
	debugLogFile = CreateFile(fileName, FILE_WRITE_DATA, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (debugLogFile == INVALID_HANDLE_VALUE)
		showLastError(_T("Error opening debug log"));
}

void closeDebugLog() {
	if (debugLogFile != INVALID_HANDLE_VALUE) {
		CloseHandle(debugLogFile);
		debugLogFile = INVALID_HANDLE_VALUE;
	}
}

/* Clone the handle of the debug log file for use within the current process.
 */
void ensureLocalHandle() {
	// Check whether we have a valid handle already.
	if (localDebugLogFile == INVALID_HANDLE_VALUE && debugLogFile != INVALID_HANDLE_VALUE) {
		// Get a handle to the process that originally opened the file.
		HANDLE process = OpenProcess(PROCESS_DUP_HANDLE, FALSE, mainProcessId);
		if (process) {
			// Duplicate the handle.
			// NOTE that the duplicate handle is never closed, and remains open in the process until it terminates.
			// The file share mode is lenient, so it shouldn't be a problem.
			// It's only a debug log, after all. We shouldn't do nasty things like this in release builds :)
			DuplicateHandle(process, debugLogFile, GetCurrentProcess(), &localDebugLogFile, 0, TRUE, DUPLICATE_SAME_ACCESS);
			CloseHandle(process);
		}
	}
}

void debugLog(LPCTSTR format, ...) {
	ensureLocalHandle();
	if (localDebugLogFile != INVALID_HANDLE_VALUE) {
		const int BUF_SIZE = 512;
		TCHAR formatBuf[BUF_SIZE];
		long time = GetTickCount();
		StringCchPrintf(formatBuf, BUF_SIZE, _T("%d.%03d: %s\r\n"), time/1000, time%1000, format);

		TCHAR buffer[BUF_SIZE];
		va_list args;
		va_start(args, format);
		StringCchVPrintf(buffer, BUF_SIZE, formatBuf, args);
		va_end(args);

		DWORD written;
		size_t length;
		StringCbLength(buffer, BUF_SIZE / sizeof(buffer[0]), &length);
		WriteFile(localDebugLogFile, buffer, length, &written, NULL);
	}
}

#endif // _DEBUG