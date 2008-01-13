#ifdef _DEBUG

#include "debuglog.hpp"
#include "state.hpp"

#include <windows.h>
#include <stdio.h>

extern FILE *debugLogFile;

void openDebugLog() {
	debugLogFile = fopen("taekwindow-debug.log", "wt");
}

void closeDebugLog() {
	if (debugLogFile) {
		fclose(debugLogFile);
		debugLogFile = NULL;
	}
}

void debugLog(char const *format, ...) {
	if (debugLogFile) {
		long time = GetTickCount();
		fprintf(debugLogFile, "%d.%03d: ", time/1000, time%1000);

		va_list args;
		va_start(args, format);
		vfprintf(debugLogFile, format, args);
		va_end(args);

		fprintf(debugLogFile, "\r\n");
	}
}

#endif /* _DEBUG */