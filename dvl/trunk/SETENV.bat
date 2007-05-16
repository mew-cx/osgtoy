REM $Id$
REM configure the environment for OSG development
REM you will want to ensure/adjust this is to your liking

REM default settings ======================================================

set PROJECT=osgdvl

set ROOTDIR=%CD%
set USRTMP=C:\_tmp
if "%VS80COMNTOOLS%"=="" (
set VS80COMNTOOLS=NO_VS8_INSTALLED
)
set VSIDE="%VS80COMNTOOLS%..\IDE\devenv.exe"

REM host-specific settings ================================================

if %COMPUTERNAME%==GLOW (
set USRTMP=E:\_tmp
set VSIDE="%VS71COMNTOOLS%..\IDE\devenv.exe"
)

REM other settings ========================================================

set BUILDDIR=%USRTMP%\%PROJECT%_build
set INSTALLDIR=%USRTMP%\%PROJECT%_install

set SOLUTION=%BUILDDIR%\%PROJECT%.sln

REM OSG runtime settings ==================================================

set OSG_NOTIFY_LEVEL=INFO

set DEBUG=D

REM set OSG_GL_EXTENSION_DISABLE=GL_SGIS_generate_mipmap

REM set OSG_OPTIMIZER=OFF
REM set OSG_OPTIMIZER="FLATTEN_STATIC_TRANSFORMS CHECK_GEOMETRY"

REM executable paths ======================================================

set PATH=%ROOTDIR%\3rdParty\bin;%PATH%

set PATH=%BUILDDIR%\bin\debug;%PATH%
set PATH=%BUILDDIR%\lib\debug;%PATH%
set PATH=%BUILDDIR%\lib\osgPlugins\debug;%PATH%
REM set PATH=%INSTALLDIR%\bin;%PATH%
REM set PATH=%INSTALLDIR%\share\OpenSceneGraph\bin;%PATH%

REM set TCL84_ROOT=C:\Tcl
REM set PATH=%TCL84_ROOT%\bin;%PATH%

REM set PATH=%ROOTDIR%\osgEphemeris\bin;%PATH%

REM data paths ============================================================

set DATADIR=%ROOTDIR%\data

set OSG_FILE_PATH=%DATADIR%

set X=%DATADIR%\OpenSceneGraph-Data
set OSG_FILE_PATH=%X%;%OSG_FILE_PATH%
set OSG_FILE_PATH=%X%\Images;%OSG_FILE_PATH%
set OSG_FILE_PATH=%X%\fonts;%OSG_FILE_PATH%

set X=%ROOTDIR%\osgToy\osgtoy-data
set OSG_FILE_PATH=%X%;%OSG_FILE_PATH%
set OSG_FILE_PATH=%X%\Images;%OSG_FILE_PATH%
set OSG_FILE_PATH=%X%\shaders;%OSG_FILE_PATH%

REM show current configuration for review =================================
REM set
REM pause

REM vim: set sw=4 ts=8 et ic ai:
