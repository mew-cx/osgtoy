@echo off
call PACKAGE_VERSIONS.bat

if NOT EXIST %DIR_PNG%\png.h (
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
echo ERROR first must build libpng for "DLL Release" and "DLL Debug"
echo using %DIR_PNG%\projects\visualc80\libpng.sln
echo then rerun this script.
pause
exit
)

echo copying products to %INSTALL_ROOT%
echo on

copy %DIR_PNG%\projects\visualc80\Win32_DLL_Release\libpng13.dll %INSTALL_BIN%
copy %DIR_PNG%\projects\visualc80\Win32_DLL_Debug\libpng13d.dll %INSTALL_BIN%

copy %DIR_PNG%\projects\visualc80\Win32_DLL_Release\libpng13.lib %INSTALL_LIB%
copy %DIR_PNG%\projects\visualc80\Win32_DLL_Debug\libpng13d.lib %INSTALL_LIB%

copy %DIR_PNG%\png.h %INSTALL_INC%
copy %DIR_PNG%\pngconf.h %INSTALL_INC%

@echo SUCCESS libpng installed
@echo be sure to install zlib too
@pause
