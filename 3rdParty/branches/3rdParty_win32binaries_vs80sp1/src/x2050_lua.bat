@echo off
call PACKAGE_VERSIONS.bat

if NOT EXIST %DIR_LUA%\src\lua.h (
echo ERROR %PKG_LUA% must be unpacked to %DIR_LUA%
pause
exit
)

if NOT EXIST %DIR_LUA%\bin\liblua51.dll (
echo ERROR first must build lua using %DIR_LUA%\vs80\lua51.sln
echo then rerun this script.
pause
exit
)

echo copying products to %INSTALL_ROOT%
echo on

copy %DIR_LUA%\bin\liblua51.dll %INSTALL_BIN%
copy %DIR_LUA%\bin\liblua51D.dll %INSTALL_BIN%
copy %DIR_LUA%\bin\lua.exe %INSTALL_BIN%
copy %DIR_LUA%\bin\luaS.exe %INSTALL_BIN%

copy %DIR_LUA%\lib\liblua51.lib %INSTALL_LIB%
copy %DIR_LUA%\lib\liblua51D.lib %INSTALL_LIB%

copy %DIR_LUA%\src\lua.h %INSTALL_INC%
copy %DIR_LUA%\src\luaconf.h %INSTALL_INC%
copy %DIR_LUA%\src\lualib.h %INSTALL_INC%
copy %DIR_LUA%\src\lauxlib.h %INSTALL_INC%
copy %DIR_LUA%\etc\lua.hpp %INSTALL_INC%

@echo SUCCESS lua installed
@pause
