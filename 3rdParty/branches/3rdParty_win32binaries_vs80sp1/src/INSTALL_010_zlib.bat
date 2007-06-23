@echo off
call PACKAGE_VERSIONS.bat

if NOT EXIST %DIR_ZLIB%\zlib.h (
echo ERROR %PKG_ZLIB% must be unpacked to %DIR_ZLIB%
pause
exit
)

if NOT EXIST %DIR_PNG%\projects\visualc80\Win32_DLL_Release\ZLib\zlib1.dll (
echo ERROR zlib will be built as part of libpng.
echo build libpng first, then rerun this script.
pause
exit
)

copy %DIR_PNG%\projects\visualc80\Win32_DLL_Release\ZLib\zlib1.dll %BIN_DIR%
copy %DIR_PNG%\projects\visualc80\Win32_DLL_Debug\ZLib\zlib1d.dll %BIN_DIR%

copy %DIR_PNG%\projects\visualc80\Win32_DLL_Release\ZLib\zlib1.lib %LIB_DIR%
copy %DIR_PNG%\projects\visualc80\Win32_DLL_Debug\ZLib\zlib1d.lib %LIB_DIR%

copy %DIR_ZLIB%\zlib.h %INC_DIR%
copy %DIR_ZLIB%\zconf.h %INC_DIR%

echo OK zlib installed
pause
