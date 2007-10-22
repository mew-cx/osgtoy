REM $Id$
REM envars to define OpenSceneGraph development environment

REM project ===============================================================

set PROJECT=osg

set VC_CFG=Debug
set DEBUG=D

REM set VC_CFG=Release
REM set DEBUG=

set VC_SLN=%PROJECT%.sln
REM set SOLUTION=%BUILDDIR%\%PROJECT%.sln

set SOURCEDIR=%CD%
set BUILDDIR=%SOURCEDIR%\_BUILD_%PROJECT%_%VC_CFG%
set INSTALLDIR=%SOURCEDIR%\_INSTALL_%PROJECT%_%VC_CFG%

REM tools =================================================================

set VSIDE="%VS80COMNTOOLS%..\IDE\devenv.exe"
set CMAKEDIR=C:\Program Files\CMake 2.4\bin
set CM_GEN=-G "Visual Studio 8 2005"

REM glow.mew.cx
if %COMPUTERNAME%==GLOW (
    set VSIDE="%VS71COMNTOOLS%..\IDE\devenv.exe"
    set CMAKEDIR=E:\progfiles\cmake247\bin
    set CM_GEN=-G "Visual Studio 7 .NET 2003"
)

REM OSG runtime settings ==================================================

REM set OSG_NOTIFY_LEVEL=ALWAYS
REM set OSG_NOTIFY_LEVEL=FATAL
REM set OSG_NOTIFY_LEVEL=WARN
set OSG_NOTIFY_LEVEL=NOTICE
REM set OSG_NOTIFY_LEVEL=DEBUG
REM set OSG_NOTIFY_LEVEL=INFO

set OSG_WINDOW=100 100 640 480

REM set OSG_GL_EXTENSION_DISABLE=GL_SGIS_generate_mipmap

REM set OSG_OPTIMIZER=OFF
REM set OSG_OPTIMIZER="FLATTEN_STATIC_TRANSFORMS CHECK_GEOMETRY"

REM executable paths ======================================================

REM pre-built binaries
set PATH=%SOURCEDIR%\externals\3rdParty\bin;%PATH%

REM built binaries
REM set PATH=%BUILDDIR%\OpenSceneGraph\bin\%VC_CFG%;%PATH%
REM set PATH=%BUILDDIR%\OpenSceneGraph\lib\%VC_CFG%;%PATH%
REM set PATH=%BUILDDIR%\OpenSceneGraph\lib\osgPlugins\%VC_CFG%;%PATH%
REM set PATH=%BUILDDIR%\osgToy\bin\%VC_CFG%;%PATH%
REM set PATH=%BUILDDIR%\osgToy\lib\%VC_CFG%;%PATH%
REM set PATH=%BUILDDIR%\osgVRPN\bin\%VC_CFG%;%PATH%
REM set PATH=%BUILDDIR%\osgVRPN\lib\%VC_CFG%;%PATH%

REM installed binaries
set PATH=%INSTALLDIR%\bin;%PATH%
set PATH=%INSTALLDIR%\share\OpenSceneGraph\bin;%PATH%

REM set PATH=%OSG_PATH%;%PATH%

REM data paths ============================================================

set OSG_FILE_PATH=

set X=%SOURCEDIR%\externals\OpenSceneGraph-Data
set OSG_FILE_PATH=%X%;%OSG_FILE_PATH%
set OSG_FILE_PATH=%X%\Images;%OSG_FILE_PATH%
set OSG_FILE_PATH=%X%\fonts;%OSG_FILE_PATH%

set X=%SOURCEDIR%\externals\osgToy-Data
set OSG_FILE_PATH=%X%;%OSG_FILE_PATH%
set OSG_FILE_PATH=%X%\Images;%OSG_FILE_PATH%
set OSG_FILE_PATH=%X%\shaders;%OSG_FILE_PATH%

set X=

REM show current configuration for review =================================
REM set
REM pause




REM vim: set sw=4 ts=8 et ic ai:
