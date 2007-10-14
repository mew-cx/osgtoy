@echo off
title running vcbuild...
call SETENV.bat
call "%VS80COMNTOOLS%vsvars32.bat"

cd %BUILDDIR%
start %VSIDE% %VC_SLN%
