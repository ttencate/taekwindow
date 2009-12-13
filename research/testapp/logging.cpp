#define _CRT_SECURE_NO_DEPRECATE 1

#include "testapp.hpp"

FILE *logfile;
int indentLevel;
char indentation[256];

void startLogging() {
	logfile = fopen("victim.log", "wt");
	indentLevel = 0;
	indentation[0] = '\0';
}

void endLogging() {
	fclose(logfile);
}

void indent() {
	indentation[indentLevel] = '\t';
	indentLevel++;
	indentation[indentLevel] = '\0';
}

void outdent() {
	if (indentLevel > 0)
		indentLevel--;
	indentation[indentLevel] = '\0';
}

void log(const char *format, ...) {
	int time = GetTickCount();

	size_t len = strlen(format);
	char *buffer = new char[len + 256];
	sprintf(buffer, "%d.%03d : %s%s\n", time/1000, time%1000, indentation, format);

	va_list args;
	va_start(args, format);
	vfprintf(logfile, buffer, args);
	va_end(args);

	delete buffer;
}

