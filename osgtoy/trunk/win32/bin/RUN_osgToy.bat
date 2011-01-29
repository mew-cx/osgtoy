@echo off
@title %0
@set PATH=%OSG_ROOT%\bin;%PATH%
@cls
@echo Some examples from osgToy
@echo Press ESCAPE to advance to the next demo

@echo The Lorenz attractor...
osgviewer 20000.lorenz

@echo A level-3 Menger sponge...
osgviewer 3.sponge

@echo visualize and diagnose normals...
osgviewer cessna.osg.drawnorm

@echo some of the many SuperShapes, using the .lua plugin...
osgviewer ss3d_default.lua
osgviewer ss3d_urchin.lua

@echo using GlslLint to catch GLSL shader source errors...
osgviewer -O verbose glsllint_errors.osg.glsllint

@echo the same model with 2 kinds of reconstructed normals...
osgviewer no_normals.osg.-3,0,0.trans  no_normals.osg.smoother  no_normals.osg.faceter.3,0,0.trans

@echo animated spin of a model...
osgviewer cessna.osg cow.osg.spin

@echo simplify the model by 90 percent...
osgviewer cessna.osg.10.simplifier

@echo earth in the celestial sphere...
osgviewer SolarSystem/earth_clouds256128.jpg.1.globe starsphere.osg

