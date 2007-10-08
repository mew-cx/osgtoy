@echo off
call PACKAGE_VERSIONS.bat

if NOT EXIST %DIR_FREETYPE%\include\ft2build.h (
echo ERROR %PKG_FREETYPE% must be unpacked to %DIR_FREETYPE%
pause
exit
)

if NOT EXIST %DIR_FREETYPE%\objs\freetype235.lib (
echo ERROR first must build freetype using %DIR_FREETYPE%\builds\win32\visualc\freetype.sln
echo then rerun this script.
pause
exit
)

echo copying products to %INSTALL_ROOT%
echo on

copy %DIR_FREETYPE%\objs\freetype235.lib %INSTALL_LIB%
copy %DIR_FREETYPE%\objs\freetype235_D.lib %INSTALL_LIB%

xcopy /e %DIR_FREETYPE%\include\* %INSTALL_INC%

@echo SUCCESS freetype installed
@pause
