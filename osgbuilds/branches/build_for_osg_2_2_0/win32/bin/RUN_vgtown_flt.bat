@echo off
REM the "Vega Town" database is included with the VegaPlayer demo
REM http://www.multigen-paradigm.com/support/sc_files/VegaPlayer_NT.zip

set PATH=%OSG_ROOT%\bin;%OSG_ROOT%\share\OpenSceneGraph\bin;%PATH%

set OSG_FILE_PATH=E:\mew\osg\tmp\VegaPlayer_NT\demo;%OSG_FILE_PATH%
osgviewer%DEBUG% town.flt
