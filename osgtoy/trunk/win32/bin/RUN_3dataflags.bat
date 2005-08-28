@echo off
title %0
call SETENV.bat

set OSG_OPTIMIZER=OFF
REM set DEBUG=
set FILE=glsl_dataflag.osg
osgviewer%DEBUG% %FILE% %FILE%.3,0,0.trans %FILE%.6,3,3.trans xyz1.osg

