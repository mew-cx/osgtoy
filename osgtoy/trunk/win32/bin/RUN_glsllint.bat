@echo off
title %0

set PATH=%OSG_ROOT%\bin;%PATH%
set OSG_NOTIFY_LEVEL=INFO

REM osgviewer glsl_simple.osg.glsllint
REM osgviewer compactdisc.osg.glsllint
osgviewer glsllint_errors.osg.glsllint

REM osgviewer -O verbose glsl_simple.osg.glsllint
REM osgviewer -O verbose compactdisc.osg.glsllint
osgviewer -O verbose glsllint_errors.osg.glsllint

