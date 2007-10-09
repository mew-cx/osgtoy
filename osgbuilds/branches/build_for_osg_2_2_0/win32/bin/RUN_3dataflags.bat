@echo off
title %0
set PATH=%OSG_ROOT%\bin;%OSG_ROOT%\share\OpenSceneGraph\bin;%PATH%

REM Each glsl_dataflag displays the contents of its modelview matrix.
REM However, optimization can rearrange the scene, thus affecting the results:
REM 3 separate dataflag geometries, each with their own modelview matrix, or
REM 1 merged geometry of all 3 dataflags, with a single modelview matrix.
set OSG_OPTIMIZER=OFF

set FILE=glsl_dataflag.osg
osgviewer%DEBUG% %FILE% %FILE%.3,0,0.trans %FILE%.6,3,3.trans xyz1.osg
