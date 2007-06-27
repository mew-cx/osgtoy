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

echo copying products to %INSTALL_ROOT%
echo on

copy %DIR_PNG%\projects\visualc80\Win32_DLL_Release\ZLib\zlib1.dll %INSTALL_BIN%
copy %DIR_PNG%\projects\visualc80\Win32_DLL_Debug\ZLib\zlib1d.dll %INSTALL_BIN%

copy %DIR_PNG%\projects\visualc80\Win32_DLL_Release\ZLib\zlib1.lib %INSTALL_LIB%
copy %DIR_PNG%\projects\visualc80\Win32_DLL_Debug\ZLib\zlib1d.lib %INSTALL_LIB%

copy %DIR_ZLIB%\zlib.h %INSTALL_INC%
copy %DIR_ZLIB%\zconf.h %INSTALL_INC%

@echo SUCCESS zlib installed
@pause
