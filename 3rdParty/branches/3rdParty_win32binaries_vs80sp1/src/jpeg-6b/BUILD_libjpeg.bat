
call "C:\Program Files\Microsoft Visual Studio 8\VC\bin\vcvars32.bat"

copy jconfig.vc jconfig.h

del *.obj
nmake clean
nmake DFLAG= NODEBUG=1
copy libjpeg.lib ..\..\lib

del *.obj
nmake clean
nmake DFLAG=D 
copy libjpegD.lib ..\..\lib

copy jpeglib.h ..\..\include
copy jerror.h ..\..\include
copy jconfig.h ..\..\include
copy jmorecfg.h ..\..\include

pause
