REM $Id$
REM Copy "SETENV.bat.tmpl" to "SETENV.bat" and customize as desired.

REM basic configuration ===================================================

set PROJECT=osg197

REM set BUILDCFG=Debug
REM set DEBUG=D

set BUILDCFG=Release
set DEBUG=

REM defaults ==============================================================

set ROOTDIR=%CD%
set USRTMP=C:\_tmp

set VSIDE="VS_NOT_FOUND"
if NOT "%VS80COMNTOOLS%"=="" (
    set VSIDE="%VS80COMNTOOLS%..\IDE\devenv.exe"
)

set CMAKE="C:\Program Files\CMake 2.4\bin\cmake.exe"

REM host-specific overrides ===============================================

REM glow.mew.cx
if %COMPUTERNAME%==GLOW (
    set USRTMP=E:\_tmp
    set VSIDE="%VS71COMNTOOLS%..\IDE\devenv.exe"
    set CMAKE="E:\progfiles\CMake_2.4.6\bin\cmake.exe"
)

REM project settings ======================================================

set BUILDDIR=%USRTMP%\%PROJECT%_BUILD
set SOLUTION=%BUILDDIR%\%PROJECT%.sln

set INSTALLDIR=%USRTMP%\%PROJECT%_INSTALL_%BUILDCFG%

REM OSG runtime settings ==================================================


set OSG_NOTIFY_LEVEL=INFO

REM set OSG_GL_EXTENSION_DISABLE=GL_SGIS_generate_mipmap

REM set OSG_OPTIMIZER=OFF
REM set OSG_OPTIMIZER="FLATTEN_STATIC_TRANSFORMS CHECK_GEOMETRY"

REM executable paths ======================================================

REM pre-built binaries
set PATH=%ROOTDIR%\3rdParty\bin;%PATH%

REM built binaries
set PATH=%BUILDDIR%\OpenSceneGraph\bin\%BUILDCFG%;%PATH%
set PATH=%BUILDDIR%\OpenSceneGraph\lib\%BUILDCFG%;%PATH%
set PATH=%BUILDDIR%\OpenSceneGraph\lib\osgPlugins\%BUILDCFG%;%PATH%
REM set PATH=%BUILDDIR%\osgToy\bin\%BUILDCFG%;%PATH%
REM set PATH=%BUILDDIR%\osgToy\lib\%BUILDCFG%;%PATH%
REM set PATH=%BUILDDIR%\osgVRPN\bin\%BUILDCFG%;%PATH%
REM set PATH=%BUILDDIR%\osgVRPN\lib\%BUILDCFG%;%PATH%

REM installed binaries
REM set PATH=%INSTALLDIR%\bin;%PATH%
REM set PATH=%INSTALLDIR%\share\OpenSceneGraph\bin;%PATH%

REM data paths ============================================================

set DATADIR=%ROOTDIR%\data

set OSG_FILE_PATH=%DATADIR%

set X=%DATADIR%\OpenSceneGraph-Data
set OSG_FILE_PATH=%X%;%OSG_FILE_PATH%
set OSG_FILE_PATH=%X%\Images;%OSG_FILE_PATH%
set OSG_FILE_PATH=%X%\fonts;%OSG_FILE_PATH%

set X=%DATADIR%\osgtoy-data
set OSG_FILE_PATH=%X%;%OSG_FILE_PATH%
set OSG_FILE_PATH=%X%\Images;%OSG_FILE_PATH%
set OSG_FILE_PATH=%X%\shaders;%OSG_FILE_PATH%

set X=

REM show current configuration for review =================================
REM set
REM pause

REM vim: set sw=4 ts=8 et ic ai:
