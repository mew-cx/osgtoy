@echo off
call PACKAGE_VERSIONS.bat

set SRCDIR=%CD%\%DIR_ZLIB%

if NOT EXIST %SRCDIR%\zlib.h (
echo ERROR %PKG_ZLIB% must be unpacked to %SRCDIR%
pause
exit
)

if NOT EXIST foo (
echo ERROR zlib will be built as part of libpng.
echo build libpng first, then rerun this script.
pause
exit
)

echo TODO do copy here

echo OK zlib installed
pause
