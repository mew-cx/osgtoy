REM fetch upstream source packages from the 'net

call PACKAGE_VERSIONS.bat

set WGET=c:\mew\bin\wget

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

pause
