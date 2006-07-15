@echo off
REM the "Vega Town" database is included with the VegaPlayer demo
REM http://www.multigen-paradigm.com/support/sc_files/VegaPlayer_NT.zip

set PATH=%OSG_ROOT%\bin;%PATH%
set X=E:\TMP\DOWNLOAD\VegaPlayer_NT
set OSG_FILE_PATH=%X%\demo;%X%\data;%OSG_FILE_PATH%
osgviewer%DEBUG% town.flt
