@echo off
title %0
call SETENV.bat

echo Running VisualStudio batch build, please wait...
echo solution = %SOLUTION%
echo buildconfig = %BUILDCFG%
date /t
time /t

%VSIDE% /build %BUILDCFG% /project INSTALL %SOLUTION%

echo DONE
date /t
time /t
pause
