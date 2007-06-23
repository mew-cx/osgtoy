@echo off

call PACKAGE_VERSIONS.bat

set SRCDIR=%CD%\%DIR_ZLIB%

if NOT EXIST %SRCDIR% (
echo ERROR %PKG_ZLIB% must be unpacked to %SRCDIR%
pause
exit
)

if NOT EXIST foo (
echo ERROR zlib is built as part of libpng.
echo rerun this script after building libpng.
pause
exit
)

echo OK zlib successfully installed
pause
