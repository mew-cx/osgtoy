@echo off
call PACKAGE_VERSIONS.bat

if NOT EXIST %DIR_JPEG%\jpeglib.h (
echo ERROR %PKG_JPEG% must be unpacked to %DIR_JPEG%
pause
exit
)

call "%VS80COMNTOOLS%vsvars32.bat"

cd %DIR_JPEG%

copy jconfig.vc jconfig.h

nmake clean
nmake DFLAG= NODEBUG=1
copy libjpeg.lib %INSTALL_LIB%

nmake clean
nmake DFLAG=D 
copy libjpegD.lib %INSTALL_LIB%

echo copying products to %INSTALL_ROOT%
echo on

copy jpeglib.h %INSTALL_INC%
copy jerror.h %INSTALL_INC%
copy jconfig.h %INSTALL_INC%
copy jmorecfg.h %INSTALL_INC%

@echo SUCCESS libjpeg installed
@pause
