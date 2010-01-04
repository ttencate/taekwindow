#ifndef PACKAGE_H
#define PACKAGE_H

#include "../meta.h"
#include "../metautil.h"

#define ZIP_BASE_NAME        APPLICATION_BASE_NAME
#define BINARY_ZIP_FILE      ZIP_BASE_NAME-APPLICATION_VERSION_FILENAME.zip
#define SOURCE_ZIP_FILE      ZIP_BASE_NAME-APPLICATION_VERSION_FILENAME-src.zip

/* Relative to Release directory */
#define BINARY_FILES         \
	taekwindow.exe \
	readme.html
/* Relative to $(SolutionDir) */
#define SOURCE_FILES         \
	bin\*.exe \
	doc\*.vcproj \
	doc\source.txt \
	doc\*.xslt \
	graphics\gui\*.png \
	graphics\icons\*.ico \
	graphics\intermediate\*.png \
	graphics\sources\*.svg \
	graphics\sources\*.psd \
	package\*.vcproj \
	package\*.xslt \
	src\*.vcproj \
	src\*.cpp \
	src\*.hpp \
	src\*.xslt \
	src\*.rc \
	*.sln \
	*.vcproj \
	*.xml \
	*.xslt

#define SEVENZIP_FILE        7za.exe
#define SEVENZIP             BIN_DIR\SEVENZIP_FILE

#if APPLICATION_BUILD > 0
#define APPLICATION_VERSION_FILENAME APPLICATION_MAJOR_VERSION.APPLICATION_MINOR_VERSION.APPLICATION_REVISION.APPLICATION_BUILD
#elif APPLICATION_REVISION > 0
#define APPLICATION_VERSION_FILENAME APPLICATION_MAJOR_VERSION.APPLICATION_MINOR_VERSION.APPLICATION_REVISION
#else
#define APPLICATION_VERSION_FILENAME APPLICATION_MAJOR_VERSION.APPLICATION_MINOR_VERSION
#endif

#define BINARY_ZIP_FILE_ABS  OUT_DIR\BINARY_ZIP_FILE
#define SOURCE_ZIP_FILE_ABS  OUT_DIR\SOURCE_ZIP_FILE

#endif
