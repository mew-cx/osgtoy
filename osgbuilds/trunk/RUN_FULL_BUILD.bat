@echo off
title %0
call SETENV.bat

echo START
date /t
time /t
echo .

echo Running full VisualStudio batch build:
echo solution = %SOLUTION%
echo buildconfig = %BUILDCFG%
%VSIDE% /build %BUILDCFG% /project INSTALL %SOLUTION%

echo .
echo DONE
date /t
time /t
pause
