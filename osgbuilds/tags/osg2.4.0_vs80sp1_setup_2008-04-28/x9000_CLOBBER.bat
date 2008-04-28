@echo off
title IMPORTANT!
call SETENV.bat

echo NOTICE: you are about to delete %BUILDDIR%
pause

@echo on
rmdir /q /s %BUILDDIR%
@echo off

pause
