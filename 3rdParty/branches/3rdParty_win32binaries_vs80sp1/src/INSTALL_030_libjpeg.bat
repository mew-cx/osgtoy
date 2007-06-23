@echo off
call PACKAGE_VERSIONS.bat

if NOT EXIST %DIR_JPEG%\jpeglib.h (
echo ERROR %PKG_JPEG% must be unpacked to %DIR_JPEG%
pause
exit
)

call "C:\Program Files\Microsoft Visual Studio 8\VC\bin\vcvars32.bat"

cd %DIR_JPEG%

copy jconfig.vc jconfig.h

del *.obj
nmake clean
nmake DFLAG= NODEBUG=1
copy libjpeg.lib %LIB_DIR%

del *.obj
nmake clean
nmake DFLAG=D 
copy libjpegD.lib %LIB_DIR%

copy jpeglib.h %INC_DIR%
copy jerror.h %INC_DIR%
copy jconfig.h %INC_DIR%
copy jmorecfg.h %INC_DIR%

echo OK libjpeg installed
pause
