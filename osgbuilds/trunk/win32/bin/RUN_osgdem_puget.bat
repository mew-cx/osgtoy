REM Demonstration of osgdem, derived from:
REM http://www.openscenegraph.org/index.php?page=UserGuides.Osgdem
REM Puget Sound source data from:
REM http://www.cc.gatech.edu/projects/large_models/ps.html
REM mew 2005-11-21

title %0

set PATH=%OSG_PATH%;%PATH%

REM where to find the source data
set SRC_DIR=C:\mew\data\www.cc.gatech.edu\data_files\large_models\ps
set SRC_ELE=%SRC_DIR%\ps_height_16k.png
set SRC_TEX=%SRC_DIR%\ps_texture_16k.png

REM set NUM_LVL=8
set NUM_LVL=5

REM where to put the build products
set TGT_DIR=C:\mew\data\osg
set TGT_ELE=%TGT_DIR%\ps_height.tif
set TGT_TEX=%TGT_DIR%\ps_texture.tif
set TGT_IVE=%TGT_DIR%\puget%NUM_LVL%.ive

if not exist %TGT_DIR% mkdir %TGT_DIR%

if not exist %TGT_ELE% (
	gdal_translate %SRC_ELE% %TGT_ELE%
	gdaladdo -r average %TGT_ELE% 2 4 8 16 32
)

if not exist %TGT_TEX% (
	gdal_translate %SRC_TEX% %TGT_TEX%
	gdaladdo -r average %TGT_TEX% 2 4 8 16 32
)

osgdem --xx 10 --yy 10 -t %TGT_TEX% --xx 10 --yy 10 -d %TGT_ELE% -l %NUM_LVL% -v 0.1 -o %TGT_IVE%

@echo terrain generation complete.
@echo view with: osgviewer %TGT_IVE%
@pause
