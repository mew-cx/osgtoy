@echo off
title running vcbuild...
call SETENV.bat
REM call "C:\Program Files\Microsoft Visual Studio 8\VC\bin\vcvars32.bat"
call "%VS80COMNTOOLS%vsvars32.bat"

cd %BUILDDIR%
vcbuild /time %VC_SLN% "%VC_CFG%|Win32"
REM vcbuild /time /logcommands opengl32_shim.sln "Release|Win32"
REM %VSIDE% /build %BUILDCFG% /project INSTALL %SOLUTION%
pause
