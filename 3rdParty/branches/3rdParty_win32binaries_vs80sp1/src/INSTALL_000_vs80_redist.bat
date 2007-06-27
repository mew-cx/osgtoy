@echo off
call PACKAGE_VERSIONS.bat

if NOT EXIST "%DIR_REDIST%" (
echo ERROR VisualStudio 8.0 2005 redistributables not found
pause
exit
)

echo copying products to %INSTALL_ROOT%
echo on

copy "%DIR_REDIST_CRT%\*.dll" %INSTALL_BIN%
copy "%DIR_REDIST_MFC%\*.dll" %INSTALL_BIN%

@echo SUCCESS vs80_redist installed
@pause
