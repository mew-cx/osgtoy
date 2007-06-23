@echo off

call PACKAGE_VERSIONS.bat

set SRCDIR=%CD%\zlib

if NOT EXIST %SRCDIR% (
echo ERROR: %PKG_ZLIB% must be unpacked at %SRCDIR%
pause
exit
)

