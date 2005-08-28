@echo off
title %0
call SETENV.bat

set OSG_NOTIFY_LEVEL=
REM set DEBUG=

osgviewer%DEBUG% glsl_simple.osg.glsllint
osgviewer%DEBUG% compactdisc.osg.glsllint
osgviewer%DEBUG% glsllint_errors.osg.glsllint

osgviewer%DEBUG% -O verbose glsl_simple.osg.glsllint
osgviewer%DEBUG% -O verbose compactdisc.osg.glsllint
osgviewer%DEBUG% -O verbose glsllint_errors.osg.glsllint

