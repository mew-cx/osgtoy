@echo off
call PACKAGE_VERSIONS.bat

if NOT EXIST "%DIR_REDIST%" (
echo ERROR VisualStudio 8.0 2005 redistributables not found
pause
exit
)

copy "%DIR_REDIST_CRT%\*" %BIN_DIR%
copy "%DIR_REDIST_MFC%\*" %BIN_DIR%

echo OK vs80_redist installed
pause
