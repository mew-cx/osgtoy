@echo off
call PACKAGE_VERSIONS.bat

if NOT EXIST %DIR_CURL%\lib\url.c (
echo ERROR %PKG_CURL% must be unpacked to %DIR_CURL%
pause
exit
)

if NOT EXIST %DIR_CURL%\lib\Release\curllib.lib (
echo ERROR first must build curl using %DIR_CURL%\lib\curllib_mew.vcproj
echo then rerun this script.
pause
exit
)

echo copying products to %INSTALL_ROOT%
echo on

copy %DIR_CURL%\lib\Release\curllib.lib %INSTALL_LIB%
copy %DIR_CURL%\lib\Debug\curllibD.lib %INSTALL_LIB%

xcopy /e %DIR_CURL%\include\* %INSTALL_INC%

@echo SUCCESS curl installed
@pause
