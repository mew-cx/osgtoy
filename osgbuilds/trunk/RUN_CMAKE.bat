@echo off
title %0
call SETENV.bat

echo Running CMake, please wait...
date /t
time /t

if not exist %BUILDDIR% mkdir %BUILDDIR%
cd %BUILDDIR%
%CMAKE% -DCMAKE_INSTALL_PREFIX=%INSTALLDIR% -DBUILD_OSG_EXAMPLES=ON -DBUILD_OSG_WRAPPERS=ON %ROOTDIR%

echo DONE
date /t
time /t
pause
