@echo off
call SETENV.bat

osgimgconv%DEBUG% 5x7.png zzzzzzzzzz.hpp
pause

osgimgconv%DEBUG% 3x5.png zzzzzzzzzz.hpp
pause

osgimgconv%DEBUG% R64.png zzzzzzzzzz.hpp
pause

osgimgconv%DEBUG% X32.png zzzzzzzzzz.hpp
pause

osgimgconv%DEBUG% testpattern64.png zzzzzzzzzz.hpp

@echo LAST TEST
pause

