REM mew 2006-09-04
REM build lua-5.1.1\vs71\lua51.sln

REM top-level directory to be created by this process
set TGTDIR=%CD%\3rdParty

set BINDIR=%TGTDIR%\bin
set LIBDIR=%TGTDIR%\lib
set INCDIR=%TGTDIR%\include

if not exist %TGTDIR% mkdir %TGTDIR%
if not exist %BINDIR% mkdir %BINDIR%
if not exist %LIBDIR% mkdir %LIBDIR%
if not exist %INCDIR% mkdir %INCDIR%

cd lua-5.1.1

copy bin\liblua51.dll %BINDIR%
copy bin\liblua51D.dll %BINDIR%
copy bin\luaS.exe %BINDIR%\lua.exe

copy lib\liblua51.lib %LIBDIR%
copy lib\liblua51D.lib %LIBDIR%

copy src\lua.h %INCDIR%
copy src\luaconf.h %INCDIR%
copy src\lualib.h %INCDIR%
copy src\lauxlib.h %INCDIR%
copy etc\lua.hpp %INCDIR%

cd ..

pause
