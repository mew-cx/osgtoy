REM $Id: RUN_ss3d.bat,v 1.2 2005/11/16 17:15:00 mew Exp $
title %0
call OSG_ENVARS.bat

osgviewer ss3d_5star.lua
osgviewer ss3d_cube.lua
osgviewer ss3d_default.lua
osgviewer ss3d_trillium.lua
osgviewer ss3d_umbrella.lua
osgviewer ss3d_urchin.lua

