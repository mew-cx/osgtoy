@echo off
call SETENV.bat
TITLE osgShell %DEBUG%
cd %BUILDDIR%\%VC_CFG%
%COMSPEC% /K
