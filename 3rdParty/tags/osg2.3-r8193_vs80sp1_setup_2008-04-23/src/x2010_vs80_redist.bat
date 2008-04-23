@echo off
call PACKAGE_VERSIONS.bat

if NOT EXIST "%DIR_REDIST%" (
echo ERROR VisualStudio 8.0 2005 redistributables not found
pause
exit
)

echo copying products to %INSTALL_ROOT%
echo on

REM xcopy "%DIR_REDIST_CRT%" %INSTALL_BIN%
REM xcopy "%DIR_REDIST_MFC%" %INSTALL_BIN%

@echo TODO see script for details
@pause
