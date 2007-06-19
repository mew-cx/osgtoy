REM demonstration of osgdem, derived from:
REM http://www.andesengineering.com/BlueMarbleViewer/
REM mew 2005-11-22

title %0

set PATH=%OSG_ROOT%\bin;%PATH%

REM where to find the source data
set SRC_DIR=C:\mew\data\bluemarble
set SRC_EAST=%SRC_DIR%\land_shallow_topo_east.tif
set SRC_WEST=%SRC_DIR%\land_shallow_topo_west.tif

set NUM_LVL=12

REM where to put the build products
set TGT_DIR=C:\mew\data\osg\bluemarble%NUM_LVL%
set TGT_IVE=%TGT_DIR%\earth.ive

if not exist %TGT_DIR% mkdir %TGT_DIR%

osgdem --bluemarble-west -t %SRC_WEST% --bluemarble-east -t %SRC_EAST% --geocentric -l %NUM_LVL% -o %TGT_IVE%

pause
