@echo off
@title %0
@call SETENV.bat
@set OSG_NOTIFY_LEVEL=
@cls
@echo Some examples from osgToy
@echo Press ESCAPE to advance to the next demo

@echo some of the many SuperShapes, using the .lua plugin...
osgviewer%DEBUG% ss3d_default.lua
osgviewer%DEBUG% ss3d_urchin.lua

@echo using GlslLint to catch GLSL shader source errors...
osgviewer%DEBUG% -O verbose glsllint_errors.osg.glsllint

@echo The Lorenz attractor...
osgviewer%DEBUG% 20000.lorenz

@echo A level-3 Menger sponge...
osgviewer%DEBUG% 3.sponge

@echo visualize and diagnose normals...
osgviewer%DEBUG% cessna.osg.drawnorm

@echo the same model with 2 kinds of reconstructed normals...
osgviewer%DEBUG% no_normals.osg.-3,0,0.trans  no_normals.osg.smoother  no_normals.osg.faceter.3,0,0.trans

@echo animated spin of a model...
osgviewer%DEBUG% cessna.osg cow.osg.spin

@echo simplify the model by 90 percent...
osgviewer%DEBUG% cessna.osg.10.simplifier

@echo earth in the celestial sphere...
osgviewer%DEBUG% SolarSystem/earth_clouds256128.jpg.1.globe starsphere.osg

