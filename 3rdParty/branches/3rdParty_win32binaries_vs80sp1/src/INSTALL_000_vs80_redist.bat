@echo off

set SRC=C:\Program Files\Microsoft Visual Studio 8\VC\redist\x86
set DST=%CD%\..\bin

copy "%SRC%\Microsoft.VC80.CRT\*" %DST%
copy "%SRC%\Microsoft.VC80.MFC\*" %DST%

echo OK vs80_redist installed
pause
