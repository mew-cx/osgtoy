@echo off
title %0
call SETENV.bat

date /t
time /t
echo Running CMake, please wait...

if not exist %BUILDDIR% mkdir %BUILDDIR%
cd %BUILDDIR%
%CMAKE% -DCMAKE_INSTALL_PREFIX=%INSTALLDIR% -DBUILD_OSG_EXAMPLES=ON -DBUILD_OSG_WRAPPERS=ON %ROOTDIR%

echo DONE
date /t
time /t
pause
