@echo off
title %0

REM glsllint examines GLSL source code for correctness.
REM The glsllint_errors.osg file contains _intentional_ errors,
REM specifically to be detected by glsllint.

set PATH=%OSG_ROOT%\bin;%PATH%
set OSG_NOTIFY_LEVEL=INFO

REM osgviewer glsl_simple.osg.glsllint
REM osgviewer compactdisc.osg.glsllint
osgviewer glsllint_errors.osg.glsllint

REM osgviewer -O verbose glsl_simple.osg.glsllint
REM osgviewer -O verbose compactdisc.osg.glsllint
osgviewer -O verbose glsllint_errors.osg.glsllint

