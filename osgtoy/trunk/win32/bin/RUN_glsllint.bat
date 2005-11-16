@echo off
title %0
call OSG_ENVARS.bat

set OSG_NOTIFY_LEVEL=INFO

osgviewer glsl_simple.osg.glsllint
osgviewer compactdisc.osg.glsllint
osgviewer glsllint_errors.osg.glsllint

osgviewer -O verbose glsl_simple.osg.glsllint
osgviewer -O verbose compactdisc.osg.glsllint
osgviewer -O verbose glsllint_errors.osg.glsllint

