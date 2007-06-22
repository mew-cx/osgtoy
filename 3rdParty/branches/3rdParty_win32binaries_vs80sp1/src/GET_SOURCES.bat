REM fetch 3rdParty upstream source archives from the net using wget.

set WGET=c:\mew\bin\wget

%WGET% http://www.zlib.net/zlib123.zip
%WGET% http://downloads.sourceforge.net/libpng/lpng1218.zip
%WGET% ftp://ftp.simtel.net/pub/simtelnet/msdos/graphics/jpegsr6.zip
%WGET% http://downloads.sourceforge.net/libungif/libungif-4.1.4.tar.gz
%WGET% http://downloads.sourceforge.net/freetype/ft234.zip
%WGET% http://www.xmission.com/~nate/glut/glut-3.7.6-src.zip
%WGET% http://www.lua.org/ftp/lua-5.1.2.tar.gz

%WGET% ftp://ftp.remotesensing.org/pub/libtiff/tiff-3.8.2.zip
%WGET% ftp://ftp.remotesensing.org/proj/proj-4.5.0.zip
%WGET% http://download.osgeo.org/gdal/gdal141.zip
%WGET% ftp://ftp.cs.unc.edu/pub/packages/GRIP/vrpn/vrpn_07_07.zip
%WGET% http://developer.3dlabs.com/downloads/glslcompiler/OpenGLCompilerSept202005.zip

pause
