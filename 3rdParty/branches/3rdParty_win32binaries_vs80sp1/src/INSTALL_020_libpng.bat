@echo off
call PACKAGE_VERSIONS.bat

if NOT EXIST %DIR_PNG%\projects\visualc71\libpng.sln (
echo ERROR %PKG_PNG% must be unpacked to %DIR_PNG%
pause
exit
)

if NOT EXIST %DIR_ZLIB%\zlib.h (
echo ERROR libpng builds zlib as well.
echo ERROR %PKG_ZLIB% must be unpacked to %DIR_ZLIB%
pause
exit
)

if NOT EXIST %DIR_PNG%\projects\visualc80\Win32_DLL_Release\libpng13.dll (
echo ERROR first must build libpng using %DIR_PNG%\projects\visualc80\libpng.sln
echo then rerun this script.
pause
exit
)

copy %DIR_PNG%\projects\visualc80\Win32_DLL_Release\libpng13.dll %BIN_DIR%
copy %DIR_PNG%\projects\visualc80\Win32_DLL_Debug\libpng13d.dll %BIN_DIR%

copy %DIR_PNG%\projects\visualc80\Win32_DLL_Release\libpng13.lib %LIB_DIR%
copy %DIR_PNG%\projects\visualc80\Win32_DLL_Debug\libpng13d.lib %LIB_DIR%

copy %DIR_PNG%\png.h %INC_DIR%
copy %DIR_PNG%\pngconf.h %INC_DIR%

echo OK libpng installed
echo be sure to install zlib too
pause
