REM launch Visual Studio IDE in interactive mode
call SETENV.bat
cd %BUILDDIR%
start %VSIDE% %VC_SLN%
