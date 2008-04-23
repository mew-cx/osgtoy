REM launch Visual Studio in noninteractive/batch build mode.
title vcbuild
call SETENV.bat
cd %BUILDDIR%
vcbuild /time /logcommands %VC_SLN% "%VC_CFG%|Win32"
REM %VSIDE% /build %BUILDCFG% /project INSTALL %SOLUTION%
pause
