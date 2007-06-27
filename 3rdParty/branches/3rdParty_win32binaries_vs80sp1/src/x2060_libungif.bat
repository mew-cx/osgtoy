@echo off
call PACKAGE_VERSIONS.bat

if NOT EXIST %DIR_UNGIF%\lib\gif_lib.h (
echo ERROR %PKG_UNGIF% must be unpacked to %DIR_UNGIF%
pause
exit
)

if NOT EXIST %DIR_UNGIF%\windows\Release\libungif.lib (
echo ERROR first must build libungif using %DIR_UNGIF%\windows\libungif.sln
pause
exit
)

echo copying products to %INSTALL_ROOT%
echo on

copy %DIR_UNGIF%\windows\Release\libungif.lib  %INSTALL_LIB%
copy %DIR_UNGIF%\windows\Debug\libungifD.lib  %INSTALL_LIB%

copy %DIR_UNGIF%\lib\gif_lib.h  %INSTALL_INC%

@echo SUCCESS libungif installed
@pause
