@echo off
call PACKAGE_VERSIONS.bat

if NOT EXIST %DIR_VRPN%\vrpn\vrpn_Types.h (
echo ERROR %PKG_VRPN% must be unpacked to %DIR_VRPN%
pause
exit
)

if NOT EXIST %DIR_VRPN%\vrpn\pc_win32\Release\vrpn.lib (
echo ERROR first must build vrpn_server and vrpn_print_devices
echo using %DIR_VRPN%\vrpn_mew.sln
echo then rerun this script.
pause
exit
)

echo copying products to %INSTALL_ROOT%
echo on

copy %DIR_VRPN%\vrpn\pc_win32\server_src\vrpn_server\Release\vrpn_server.exe %INSTALL_BIN%
copy %DIR_VRPN%\vrpn\pc_win32\client_src\vrpn_print_devices\Release\vrpn_print_devices.exe %INSTALL_BIN%

copy %DIR_VRPN%\vrpn\pc_win32\Release\vrpn.lib %INSTALL_LIB%
copy %DIR_VRPN%\vrpn\pc_win32\Debug\vrpnD.lib %INSTALL_LIB%

copy %DIR_VRPN%\vrpn\vrpn_Analog.h %INSTALL_INC%
copy %DIR_VRPN%\vrpn\vrpn_BaseClass.h %INSTALL_INC%
copy %DIR_VRPN%\vrpn\vrpn_Button.h %INSTALL_INC%
copy %DIR_VRPN%\vrpn\vrpn_Configure.h %INSTALL_INC%
copy %DIR_VRPN%\vrpn\vrpn_Connection.h %INSTALL_INC%
copy %DIR_VRPN%\vrpn\vrpn_Dial.h %INSTALL_INC%
copy %DIR_VRPN%\vrpn\vrpn_Shared.h %INSTALL_INC%
copy %DIR_VRPN%\vrpn\vrpn_Tracker.h %INSTALL_INC%
copy %DIR_VRPN%\vrpn\vrpn_Types.h %INSTALL_INC%

@echo SUCCESS VRPN installed
@pause
