REM list of exact 3rdParty source packages

REM where products will be installed...
set INSTALL_ROOT=%CD%\..
set INSTALL_BIN=%INSTALL_ROOT%\bin
set INSTALL_INC=%INSTALL_ROOT%\include
set INSTALL_LIB=%INSTALL_ROOT%\lib


REM the upstream sources...

set DIR_REDIST=C:\Program Files\Microsoft Visual Studio 8\VC\redist\x86
set DIR_REDIST_CRT=%DIR_REDIST%\Microsoft.VC80.CRT
set DIR_REDIST_MFC=%DIR_REDIST%\Microsoft.VC80.MFC

set DIR_ZLIB=zlib
set PKG_ZLIB=%DIR_ZLIB%123.zip
set URL_ZLIB=http://www.zlib.net/%PKG_ZLIB%

set DIR_PNG=lpng1218
set PKG_PNG=%DIR_PNG%.zip
set URL_PNG=http://downloads.sourceforge.net/libpng/%PKG_PNG%

set DIR_JPEG=jpeg-6b
set PKG_JPEG=jpegsr6.zip
set URL_JPEG=ftp://ftp.simtel.net/pub/simtelnet/msdos/graphics/%PKG_JPEG%

set DIR_LUA=lua-5.1.2
set PKG_LUA=%DIR_LUA%.tar.gz
set URL_LUA=http://www.lua.org/ftp/%PKG_LUA%

set DIR_UNGIF=libungif-4.1.4
set PKG_UNGIF=%DIR_UNGIF%.tar.gz
set URL_UNGIF=http://downloads.sourceforge.net/libungif/%PKG_UNGIF%

set DIR_FREETYPE=freetype-2.3.4
set PKG_FREETYPE=ft234.zip
set URL_FREETYPE=http://downloads.sourceforge.net/freetype/%PKG_FREETYPE%

set DIR_GLUT=glut-3.7.6
set PKG_GLUT=%DIR_GLUT%-src.zip
set URL_GLUT=http://www.xmission.com/~nate/glut/%PKG_GLUT%

set DIR_TIFF=tiff-3.8.2
set PKG_TIFF=%DIR_TIFF%.zip
set URL_TIFF=ftp://ftp.remotesensing.org/pub/libtiff/%PKG_TIFF%

set DIR_VRPN=vrpn_07_13
set PKG_VRPN=%DIR_VRPN%.zip
set URL_VRPN=ftp://ftp.cs.unc.edu/pub/packages/GRIP/vrpn/%PKG_VRPN%

REM ------------------------------

set DIR_GLSLFE=OpenGLCompilerSept202005
set PKG_GLSLFE=%DIR_GLSLFE%.zip
set URL_GLSLFE=http://developer.3dlabs.com/downloads/glslcompiler/%PKG_GLSLFE%

set DIR_PROJ=
set PKG_PROJ=proj-4.5.0.zip
set URL_PROJ=ftp://ftp.remotesensing.org/proj/%PKG_PROJ%

set DIR_GDAL=
set PKG_GDAL=gdal142.zip
set URL_GDAL=http://download.osgeo.org/gdal/%PKG_GDAL%

