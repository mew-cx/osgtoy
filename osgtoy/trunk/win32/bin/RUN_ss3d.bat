REM $Id: RUN_ss3d.bat,v 1.1 2005/08/28 04:27:54 mew Exp $
title %0
call SETENV.bat
set OSG_NOTIFY_LEVEL=

osgviewer%DEBUG% ss3d_5star.lua
osgviewer%DEBUG% ss3d_cube.lua
osgviewer%DEBUG% ss3d_default.lua
osgviewer%DEBUG% ss3d_trillium.lua
osgviewer%DEBUG% ss3d_umbrella.lua
osgviewer%DEBUG% ss3d_urchin.lua

osgviewer%DEBUG% ss3d_0000.lua
osgviewer%DEBUG% ss3d_0002.lua
osgviewer%DEBUG% ss3d_0004.lua
osgviewer%DEBUG% ss3d_0006.lua
osgviewer%DEBUG% ss3d_0007.lua
osgviewer%DEBUG% ss3d_0009.lua
osgviewer%DEBUG% ss3d_0010.lua
osgviewer%DEBUG% ss3d_0011.lua
osgviewer%DEBUG% ss3d_0012.lua
osgviewer%DEBUG% ss3d_0014.lua
osgviewer%DEBUG% ss3d_0015.lua
osgviewer%DEBUG% ss3d_0016.lua
osgviewer%DEBUG% ss3d_0017.lua
osgviewer%DEBUG% ss3d_0019.lua
osgviewer%DEBUG% ss3d_0020.lua
osgviewer%DEBUG% ss3d_0026.lua

