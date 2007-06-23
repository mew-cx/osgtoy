@echo off
call PACKAGE_VERSIONS.bat

set SRCDIR=%CD%\%DIR_PNG%

if NOT EXIST %SRCDIR%\projects\visualc71\libpng.sln (
echo ERROR %PKG_PNG% must be unpacked to %SRCDIR%
pause
exit
)

if NOT EXIST %SRCDIR%\zlib.h (
echo ERROR %PKG_ZLIB% must be unpacked to %SRCDIR%
pause
exit
)

if NOT EXIST foo (
echo ERROR need to build libpng using %SRCDIR%\projects\visualc80\libpng.sln
echo build first, then rerun this script.
pause
exit
)

echo TODO do copy here

echo OK libpng installed
echo be sure to install zlib too
pause
