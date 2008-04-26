@echo off
call PACKAGE_VERSIONS.bat

if NOT EXIST %DIR_TIFF%\libtiff\tiff.h (
echo ERROR %PKG_TIFF% must be unpacked to %DIR_TIFF%
pause
exit
)

call "%VS80COMNTOOLS%vsvars32.bat"

cd %DIR_TIFF%

nmake /f Makefile.vc clean
nmake /f Makefile.vc NODEBUG=1 lib
copy libtiff\libtiff.lib %INSTALL_LIB%

nmake /f Makefile.vc clean
nmake /f Makefile.vc lib
copy libtiff\libtiffD.lib %INSTALL_LIB%

echo copying products to %INSTALL_ROOT%
echo on

copy libtiff\tiff.h %INSTALL_INC%
copy libtiff\tiffconf.h %INSTALL_INC%
copy libtiff\tiffio.h %INSTALL_INC%
copy libtiff\tiffvers.h %INSTALL_INC%

@echo SUCCESS libtiff installed
@pause
