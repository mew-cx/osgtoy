@echo off
call PACKAGE_VERSIONS.bat

set WGET=e:\mew\bin\wget.exe

cd UPSTREAM_SOURCES

%WGET% %URL_ZLIB%
%WGET% %URL_PNG%
%WGET% %URL_JPEG%
%WGET% %URL_LUA%
%WGET% %URL_UNGIF%
%WGET% %URL_FREETYPE%
%WGET% %URL_GLUT%
%WGET% %URL_TIFF%
%WGET% %URL_VRPN%
%WGET% %URL_GLSLFE%
%WGET% %URL_PROJ%
%WGET% %URL_GDAL%
%WGET% %URL_GEOTIFF%

echo SUCCESS source packages downloaded
pause
