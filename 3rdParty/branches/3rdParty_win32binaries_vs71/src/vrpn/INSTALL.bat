REM mew 2005-04-29

REM directory to be created by this process
set TGTDIR=%CD%\3rdParty

set BINDIR=%TGTDIR%\bin
set LIBDIR=%TGTDIR%\lib
set INCDIR=%TGTDIR%\include

if not exist %TGTDIR% mkdir %TGTDIR%
if not exist %BINDIR% mkdir %BINDIR%
if not exist %LIBDIR% mkdir %LIBDIR%
if not exist %INCDIR% mkdir %INCDIR%

set HERE=%CD%

REM =================================================================
REM vrpn_06_06\vrpn\vrpn.dsw
REM build: quatlib, vrpn, vrpn_print_devices, vrpn_server

cd vrpn_06_06

copy vrpn\pc_win32\server_src\vrpn_server\Release\vrpn_server.exe %BINDIR%
copy vrpn\pc_win32\client_src\vrpn_print_devices\Release\vrpn_print_devices.exe %BINDIR%

copy vrpn\pc_win32\Release\vrpn.lib %LIBDIR%
copy vrpn\pc_win32\Debug\vrpnD.lib %LIBDIR%

copy vrpn\vrpn_Analog.h %INCDIR%
copy vrpn\vrpn_BaseClass.h %INCDIR%
copy vrpn\vrpn_Button.h %INCDIR%
copy vrpn\vrpn_Configure.h %INCDIR%
copy vrpn\vrpn_Connection.h %INCDIR%
copy vrpn\vrpn_Dial.h %INCDIR%
copy vrpn\vrpn_Shared.h %INCDIR%
copy vrpn\vrpn_Tracker.h %INCDIR%
copy vrpn\vrpn_Types.h %INCDIR%

cd ..

pause
