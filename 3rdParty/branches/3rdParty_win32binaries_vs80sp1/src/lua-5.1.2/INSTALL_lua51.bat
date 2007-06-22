REM mew 2007-06-22

REM first, build all configs of lua-5.1.2 using vs80\lua51.sln
REM then...

copy bin\liblua51.dll ..\..\bin
copy bin\liblua51D.dll ..\..\bin
copy bin\lua.exe ..\..\bin
copy bin\luaS.exe ..\..\bin

copy lib\liblua51.lib ..\..\lib
copy lib\liblua51D.lib ..\..\lib

copy src\lua.h ..\..\include
copy src\luaconf.h ..\..\include
copy src\lualib.h ..\..\include
copy src\lauxlib.h ..\..\include
copy etc\lua.hpp ..\..\include

pause
