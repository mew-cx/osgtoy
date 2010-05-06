@echo off
call PACKAGE_VERSIONS.bat

if NOT EXIST %DIR_GLUT%\include\GL\glut.h (
echo ERROR %PKG_GLUT% must be unpacked to %DIR_GLUT%
pause
exit
)

if NOT EXIST %DIR_GLUT%\lib\glut\Release\glut32.dll (
echo ERROR first must build glut using %DIR_GLUT%\glut32.sln
echo then rerun this script.
pause
exit
)

echo copying products to %INSTALL_ROOT%
echo on

copy %DIR_GLUT%\lib\glut\Release\glut32.dll %INSTALL_BIN%
copy %DIR_GLUT%\lib\glut\Debug\glut32D.dll %INSTALL_BIN%

copy %DIR_GLUT%\lib\glut\Release\glut32.lib %INSTALL_LIB%
copy %DIR_GLUT%\lib\glut\Debug\glut32D.lib %INSTALL_LIB%

mkdir %INSTALL_INC%\GL
copy %DIR_GLUT%\include\GL\glut.h %INSTALL_INC%\GL

@echo SUCCESS glut installed
@pause
