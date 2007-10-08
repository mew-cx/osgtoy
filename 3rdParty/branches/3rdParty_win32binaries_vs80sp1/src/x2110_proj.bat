echo IN-PROGRESS
pause


@echo off
call PACKAGE_VERSIONS.bat

if NOT EXIST %DIR_PROJ%\vrpn\vrpn_Types.h (
echo ERROR %PKG_PROJ% must be unpacked to %DIR_PROJ%
pause
exit
)

if NOT EXIST %DIR_PROJ%\vrpn\pc_win32\Release\vrpn.lib (
echo ERROR first must build vrpn_server and vrpn_print_devices
echo using %DIR_PROJ%\vrpn_mew.sln echo then rerun this script.
pause
exit
)

echo copying products to %INSTALL_ROOT%
echo on

copy %DIR_PROJ%\TODO %INSTALL_BIN%

copy %DIR_PROJ%\TODO %INSTALL_LIB%

copy %DIR_PROJ%\TODO %INSTALL_INC%

@echo SUCCESS PROJ installed
@pause
