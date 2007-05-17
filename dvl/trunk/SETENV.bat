REM $Id$
REM configure the environment for development, adjust to your liking

REM defaults ==============================================================

set PROJECT=osgdvl
set ROOTDIR=%CD%
set USRTMP=C:\_tmp

set VSIDE="VS_NOT_FOUND"
if NOT "%VS80COMNTOOLS%"=="" (
set VSIDE="%VS80COMNTOOLS%..\IDE\devenv.exe"
)

REM host-specific overrides ===============================================

if %COMPUTERNAME%==GLOW (
set USRTMP=E:\_tmp
set VSIDE="%VS71COMNTOOLS%..\IDE\devenv.exe"
)

REM project settings ======================================================

set BUILDDIR=%USRTMP%\%PROJECT%_build
set INSTALLDIR=%USRTMP%\%PROJECT%_install
set SOLUTION=%BUILDDIR%\%PROJECT%.sln

REM OSG runtime settings ==================================================

set DEBUG=D

set OSG_NOTIFY_LEVEL=INFO

REM set OSG_GL_EXTENSION_DISABLE=GL_SGIS_generate_mipmap

REM set OSG_OPTIMIZER=OFF
REM set OSG_OPTIMIZER="FLATTEN_STATIC_TRANSFORMS CHECK_GEOMETRY"

REM executable paths ======================================================

set PATH=%ROOTDIR%\3rdParty\bin;%PATH%

set PATH=%BUILDDIR%\OpenSceneGraph\bin\debug;%PATH%
set PATH=%BUILDDIR%\OpenSceneGraph\lib\debug;%PATH%
set PATH=%BUILDDIR%\OpenSceneGraph\lib\osgPlugins\debug;%PATH%

REM set PATH=%INSTALLDIR%\bin;%PATH%
REM set PATH=%INSTALLDIR%\share\OpenSceneGraph\bin;%PATH%

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

set X=

REM show current configuration for review =================================
REM set
REM pause

REM vim: set sw=4 ts=8 et ic ai:
