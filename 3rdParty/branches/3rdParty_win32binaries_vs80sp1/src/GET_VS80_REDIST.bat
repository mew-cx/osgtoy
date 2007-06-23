REM fetch VisualStudio 8.0 redistributable libraries

set SRC=C:\Program Files\Microsoft Visual Studio 8\VC\redist\x86
set DST=%CD%\bin

copy "%SRC%\Microsoft.VC80.CRT\*" %DST%
copy "%SRC%\Microsoft.VC80.MFC\*" %DST%

pause
