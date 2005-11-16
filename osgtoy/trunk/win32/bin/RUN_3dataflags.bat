@echo off
title %0
call OSG_ENVARS.bat

set OSG_OPTIMIZER=OFF
set FILE=glsl_dataflag.osg
osgviewer %FILE% %FILE%.3,0,0.trans %FILE%.6,3,3.trans xyz1.osg

