@echo Deleting non-deliverables before InnoSetup packaging...

rmdir /s /q 3rdParty\src

del 3rdParty\bin\gdal12D.dll
del 3rdParty\bin\glslfeD.dll
del 3rdParty\bin\glut32D.dll
del 3rdParty\bin\liblua51D.dll

del 3rdParty\lib\freetype219_D.lib
del 3rdParty\lib\gdal_iD.lib
del 3rdParty\lib\glslfeD.lib
del 3rdParty\lib\glut32D.lib
del 3rdParty\lib\libjpegD.lib
del 3rdParty\lib\liblua51D.lib
del 3rdParty\lib\libpngD.lib
del 3rdParty\lib\libtiffD.lib
del 3rdParty\lib\libungifD.lib
del 3rdParty\lib\vrpnD.lib
del 3rdParty\lib\zlibD.lib

pause
