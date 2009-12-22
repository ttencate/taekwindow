#ifdef _DEBUG

#define STRSAFE_NO_DEPRECATE

#include <windows.h>
#include <tchar.h>
#include <strsafe.h>

#include "debuglog.hpp"
#include "util.hpp"

static HANDLE debugLogFile = INVALID_HANDLE_VALUE;

// TODO merge with util.[ch]pp into errors.[ch]pp
// TODO settle on printf syntax or FormatMessage syntax, not both

#define LOG_DIR _T("logs")

void openDebugLog() {
	if (!CreateDirectory(LOG_DIR, NULL)) {
		if (GetLastError() != ERROR_ALREADY_EXISTS) {
			showLastError(NULL, _T("Error creating debug log directory"));
			return;
		}
	}

	long time = GetTickCount();
	TCHAR fileName[64];
	wsprintf(fileName, LOG_DIR _T("\\taekwindow-debug-%d.%03d.log"), time/1000, time%1000);
	debugLogFile = CreateFile(fileName, FILE_WRITE_DATA, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (debugLogFile == INVALID_HANDLE_VALUE) {
		showLastError(NULL, _T("Error opening debug log"));
		return;
	}
	DEBUGLOG("Log file opened");
}

void closeDebugLog() {
	if (debugLogFile != INVALID_HANDLE_VALUE) {
		DEBUGLOG("Closing log file");
		CloseHandle(debugLogFile);
		debugLogFile = INVALID_HANDLE_VALUE;
	}
}

void debugLog(LPCTSTR format, ...) {
	if (debugLogFile != INVALID_HANDLE_VALUE) {
		const int BUF_SIZE = 512;
		TCHAR formatBuf[BUF_SIZE];
		long time = GetTickCount();
		wsprintf(formatBuf, _T("%d.%03d: %s\r\n"), time/1000, time%1000, format);

		TCHAR buffer[BUF_SIZE];
		va_list args;
		va_start(args, format);
		wvsprintf(buffer, formatBuf, args);
		va_end(args);

		DWORD written;
		size_t length;
		StringCbLength(buffer, BUF_SIZE / sizeof(buffer[0]), &length);
		WriteFile(debugLogFile, buffer, length, &written, NULL);
	}
}

void assert(bool x, TCHAR const *file, size_t line) {
	if (!x) {
		DEBUGLOG("Assertion failed in %s, line %d; aborting", file, line);
		showError(NULL, _T("Assertion failed"), _T("Assertion failed in %1!s!, line %2!u!.\r\n\r\nAborting program."), file, line);
		ExitProcess(1);
	}
}

#endif // _DEBUG