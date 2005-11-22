# Microsoft Developer Studio Project File - Name="Core osgToy" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Core osgToy - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "osgToy.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "osgToy.mak" CFG="Core osgToy - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Core osgToy - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Core osgToy - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Core osgToy - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "../../lib"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../lib"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "../../include" /I "../../../OpenThreads/include" /I "../../../Producer/include" /I "../../../3rdParty/include" /D "NDEBUG" /D "_MBCS" /D "_USRDLL" /D "OSGTOY_LIBRARY" /D "WIN32" /D "_WINDOWS" /YX /FD /Zm200 /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 glslang.lib liblualib.lib liblua.lib OpenThreadsWin32.lib glu32.lib opengl32.lib /nologo /dll /pdb:none /machine:I386 /out:"../../bin/osgToy.dll" /libpath:"../../lib" /libpath:"../../../OpenThreads/lib/win32" /libpath:"../../../Producer/lib" /libpath:"../../../3rdParty/lib"

!ELSEIF  "$(CFG)" == "Core osgToy - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../lib"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /vmg /GR /GX /Zi /Od /I "../../include" /I "../../../OpenThreads/include" /I "../../../Producer/include" /I "../../../3rdParty/include" /D "OSGTOY_LIBRARY" /D "_WINDOWS" /D "WIN32" /D "_DEBUG" /YX /FD /GZ /Zm200 /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 glslangD.lib liblualibD.lib libluaD.lib OpenThreadsWin32d.lib glu32.lib opengl32.lib /nologo /dll /debug /machine:I386 /out:"../../bin/osgToyD.dll" /pdbtype:sept /libpath:"../../lib" /libpath:"../../../OpenThreads/lib/win32" /libpath:"../../../Producer/lib" /libpath:"../../../3rdParty/lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Core osgToy - Win32 Release"
# Name "Core osgToy - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\src\osgToy\FacetingVisitor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\osgToy\LorenzAttractor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\osgToy\LuaState.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\osgToy\lua_osg.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\osgToy\lua_osgToy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\osgToy\MipmapTestImage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\osgToy\MengerSponge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\osgToy\Normals.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\osgToy\RungeKutta4.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\osgToy\SuperShape3D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\osgToy\Version.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\osgToy\GlslLint.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\osgToy\GlslLintVisitor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\osgToy\Polyhedra.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ";h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\include\osgToy\Export
# End Source File
# Begin Source File

SOURCE=..\..\include\osgToy\FacetingVisitor
# End Source File
# Begin Source File

SOURCE=..\..\include\osgToy\LorenzAttractor
# End Source File
# Begin Source File

SOURCE=..\..\include\osgToy\LuaState
# End Source File
# Begin Source File

SOURCE=..\..\include\osgToy\MipmapTestImage
# End Source File
# Begin Source File

SOURCE=..\..\include\osgToy\MengerSponge
# End Source File
# Begin Source File

SOURCE=..\..\include\osgToy\Normals
# End Source File
# Begin Source File

SOURCE=..\..\include\osgToy\PseudoLoader
# End Source File
# Begin Source File

SOURCE=..\..\include\osgToy\RungeKutta4
# End Source File
# Begin Source File

SOURCE=..\..\include\osgToy\SuperShape3D
# End Source File
# Begin Source File

SOURCE=..\..\include\osgToy\Version
# End Source File
# Begin Source File

SOURCE=..\..\include\osgToy\GlslLint
# End Source File
# Begin Source File

SOURCE=..\..\include\osgToy\GlslLintVisitor
# End Source File
# Begin Source File

SOURCE=..\..\include\osgToy\Polyhedra
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project

