REM launch Visual Studio IDE
call SETENV.bat
cd %BUILDDIR%
start %VSIDE% %VC_SLN%
