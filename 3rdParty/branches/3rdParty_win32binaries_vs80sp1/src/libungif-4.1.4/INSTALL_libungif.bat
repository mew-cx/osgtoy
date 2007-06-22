REM first, build using windows\libungif.sln
REM then...

copy windows\Debug\libungifD.lib  ..\..\lib
copy windows\Release\libungif.lib  ..\..\lib
copy lib\gif_lib.h  ..\..\include

pause
