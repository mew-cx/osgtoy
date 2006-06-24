# Microsoft Developer Studio Project File - Name="Core osgVRPN" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Core osgVRPN - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "osgVRPN.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "osgVRPN.mak" CFG="Core osgVRPN - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Core osgVRPN - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Core osgVRPN - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Core osgVRPN - Win32 Release"

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
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "../../include" /I "../../../OpenThreads/include" /I "../../../Producer/include" /I "../../../3rdParty/include" /D "NDEBUG" /D "_MBCS" /D "_USRDLL" /D "OSGVRPN_LIBRARY" /D "_WINDOWS" /D "WIN32" /YX /FD /Zm200 /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 OpenThreadsWin32.lib vrpn.lib glu32.lib opengl32.lib wsock32.lib /nologo /dll /pdb:none /machine:I386 /out:"../../bin/osgVRPN.dll" /libpath:"../../lib" /libpath:"../../../OpenThreads/lib/win32" /libpath:"../../../Producer/lib" /libpath:"../../../3rdParty/lib"

!ELSEIF  "$(CFG)" == "Core osgVRPN - Win32 Debug"

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
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /vmg /GR /GX /Z7 /Od /I "../../include" /I "../../../OpenThreads/include" /I "../../../Producer/include" /I "../../../3rdParty/include" /D "_DEBUG" /D "OSGVRPN_LIBRARY" /D "_WINDOWS" /D "WIN32" /YX /FD /GZ /Zm200 /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 OpenThreadsWin32d.lib vrpnD.lib glu32.lib opengl32.lib wsock32.lib /nologo /dll /debug /machine:I386 /out:"../../bin/osgVRPNd.dll" /pdbtype:sept /libpath:"../../lib" /libpath:"../../../OpenThreads/lib/win32" /libpath:"../../../Producer/lib" /libpath:"../../../3rdParty/lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Core osgVRPN - Win32 Release"
# Name "Core osgVRPN - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\src\osgVRPN\Analog.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\osgVRPN\AnalogTracker.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\osgVRPN\Tracker.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\osgVRPN\TrackerManipulator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\osgVRPN\TrackerTransform.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\osgVRPN\Version.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ";h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\include\osgVRPN\Analog.h
# End Source File
# Begin Source File

SOURCE=..\..\include\osgVRPN\AnalogTracker.h
# End Source File
# Begin Source File

SOURCE=..\..\include\osgVRPN\Export
# End Source File
# Begin Source File

SOURCE=..\..\include\osgVRPN\Tracker.h
# End Source File
# Begin Source File

SOURCE=..\..\include\osgVRPN\TrackerBase.h
# End Source File
# Begin Source File

SOURCE=..\..\include\osgVRPN\TrackerManipulator.h
# End Source File
# Begin Source File

SOURCE=..\..\include\osgVRPN\TrackerTransform.h
# End Source File
# Begin Source File

SOURCE=..\..\include\osgVRPN\Version
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
