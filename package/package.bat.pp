/* The includes result in many empty lines. */
REM Scroll down!

#include "package.h"

REM This must be run from the $(SolutionDir)!

/* Convenient for debugging */
@echo on

cd OUT_DIR
del BINARY_ZIP_FILE_ABS
del SOURCE_ZIP_FILE_ABS

cd OUT_DIR
SEVENZIP a -tzip -mx9 BINARY_ZIP_FILE_ABS BINARY_FILES
if errorlevel 1 goto :oops

cd SOLUTION_DIR
SEVENZIP a -tzip -mx9 SOURCE_ZIP_FILE_ABS SOURCE_FILES
if errorlevel 1 goto :oops

goto :done

:oops
echo Some kind of error occurred when creating distribution ZIPs

:done
