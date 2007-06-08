@echo off
title %0
call SETENV.bat

date /t
time /t
echo RUNNING: %VSIDE% /build %BUILDCFG% /project INSTALL %SOLUTION%
echo (any idea how to make VisualStudio show some visual progress??)

%VSIDE% /build %BUILDCFG% /project INSTALL %SOLUTION%

echo DONE
date /t
time /t
pause
