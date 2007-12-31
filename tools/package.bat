/* This file must be run through the C preprocessor.
 * It will then be a runnable .bat file that packages the program into distribution .zip files.
 */

#include "../version.inc"

#define BASENAME taekwindow
#define BINARY_ZIP BASENAME-APPLICATION_VERSION_FILENAME.zip
#define SOURCE_ZIP BASENAME-APPLICATION_VERSION_FILENAME-src.zip
#define PACKAGE_DIR package
#define SEVENZIP tools\7z.exe

mkdir package

del PACKAGE_DIR\BINARY_ZIP

REM We do not want a strange directory structure in the binary distribution,
REM so we copy everything to a temporary location.


mkdir temp
cd temp
REM No, there is no command to copy multiple files at once...
copy ..\Release\taekwindow.exe .
copy ..\Release\taekwindowhooks.dll .
copy ..\taekwindowconfig\bin\Release\taekwindowconfig.exe .
copy ..\taekwindowconfig\bin\Release\Interop.IWshRuntimeLibrary.dll .
copy ..\Readme.txt .
..\SEVENZIP a -tzip ..\PACKAGE_DIR\BINARY_ZIP
cd ..
rmdir /S /Q temp

del PACKAGE_DIR\SOURCE_ZIP
SEVENZIP a -tzip PACKAGE_DIR\SOURCE_ZIP ^
  graphics\*.ico ^
  graphics\*.ai ^
  graphics\*.psd ^
  graphics\*.png ^
  taekwindow\*.cpp ^
  taekwindow\*.h ^
  taekwindow\*.hpp ^
  taekwindow\*.rc ^
  taekwindow\*.vcproj ^
  taekwindowhooks\*.cpp ^
  taekwindowhooks\*.h ^
  taekwindowhooks\*.hpp ^
  taekwindowhooks\*.def ^
  taekwindowhooks\*.rc ^
  taekwindowhooks\*.vcproj ^
  taekwindowconfig\*.cs ^
  taekwindowconfig\*.resx ^
  taekwindowconfig\*.csproj ^
  taekwindowconfig\Properties\*.cs ^
  taekwindowconfig\Properties\*.resx ^
  tools\7z.exe ^
  tools\package.bat ^
  taekwindow.sln ^
  version.inc ^
  Readme.txt
