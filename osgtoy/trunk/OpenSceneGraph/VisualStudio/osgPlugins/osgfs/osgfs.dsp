# Microsoft Developer Studio Project File - Name="osgPlugin osgfs" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102
# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=osgPlugin osgfs - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "dot_osgfs.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "dot_osgfs.mak" CFG="osgPlugin osgfs - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "osgPlugin osgfs - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "osgPlugin osgfs - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "osgPlugin osgfs - Win32 Release Static" (based on "Win32 (x86) Static Library")
!MESSAGE "osgPlugin osgfs - Win32 Debug Static" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "osgPlugin osgfs - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../bin/$(PlatformName)"
# PROP Intermediate_Dir "$(PlatformName)/$(ConfigurationName)"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "../../../include" /I "../../../../OpenThreads/include" /I "../../../../Producer/include" /I "../../../../3rdParty/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_CRT_SECURE_NO_DEPRECATE" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 OpenThreadsWin32.lib /nologo /dll /debug /machine:I386 /nodefaultlib:"LIBC" /opt:ref /opt:icf /out:"$(OutDir)/osgdb_osgfs.dll" /implib:"../../../lib/$(PlatformName)/osgdb_osgfs.lib" /libpath:"../../../lib/$(PlatformName)" /libpath:"../../../../OpenThreads/lib/$(PlatformName)" /libpath:"../../../../Producer/lib/$(PlatformName)" /libpath:"../../../../3rdParty/lib/$(PlatformName)" /libpath:"../../../../3rdParty/lib"
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "osgPlugin osgfs - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../bin/$(PlatformName)"
# PROP Intermediate_Dir "$(PlatformName)/$(ConfigurationName)"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /vmg /GR /GX /Zi /Od /I "../../../include" /I "../../../../OpenThreads/include" /I "../../../../Producer/include" /I "../../../../3rdParty/include" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "WIN32" /D "_DEBUG" /D "_CRT_SECURE_NO_DEPRECATE" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 OpenThreadsWin32d.lib /nologo /dll /debug /machine:I386 /nodefaultlib:"LIBC" /out:"$(OutDir)/osgdb_osgfsd.dll" /pdbtype:sept /implib:"../../../lib/$(PlatformName)/osgdb_osgfsd.lib" /libpath:"../../../lib/$(PlatformName)" /libpath:"../../../../OpenThreads/lib/$(PlatformName)" /libpath:"../../../../Producer/lib/$(PlatformName)" /libpath:"../../../../3rdParty/lib/$(PlatformName)" /libpath:"../../../../3rdParty/lib"
# SUBTRACT LINK32 /pdb:none /nodefaultlib

!ELSEIF  "$(CFG)" == "osgPlugin osgfs - Win32 Release Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "../../../lib"
# PROP BASE Intermediate_Dir "Release_Static"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../lib/$(PlatformName)"
# PROP Intermediate_Dir "$(PlatformName)/$(ConfigurationName)_Static"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "OSG_LIBRARY_STATIC" /D "OT_LIBRARY_STATIC" /D "PR_LIBRARY_STATIC" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /I "../../../include" /I "../../../../OpenThreads/include" /I "../../../../Producer/include" /I "../../../../3rdParty/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "OSG_LIBRARY_STATIC" /D "OT_LIBRARY_STATIC" /D "PR_LIBRARY_STATIC" /D "_CRT_SECURE_NO_DEPRECATE" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nodefaultlib /nologo /out:"$(OutDir)/osgdb_osgfs_s.lib"
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "osgPlugin osgfs - Win32 Debug Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "../../../lib"
# PROP BASE Intermediate_Dir "Debug_Static"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../lib/$(PlatformName)"
# PROP Intermediate_Dir "$(PlatformName)/$(ConfigurationName)_Static"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "OSG_LIBRARY_STATIC" /D "OT_LIBRARY_STATIC" /D "PR_LIBRARY_STATIC" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /vmg /GR /GX /Z7 /Od /I "../../../include" /I "../../../../OpenThreads/include" /I "../../../../Producer/include" /I "../../../../3rdParty/include" /D "_WINDOWS" /D "_MBCS" /D "OSG_LIBRARY_STATIC" /D "OT_LIBRARY_STATIC" /D "PR_LIBRARY_STATIC" /D "WIN32" /D "_DEBUG" /D "_CRT_SECURE_NO_DEPRECATE" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nodefaultlib /nologo /out:"$(OutDir)/osgdb_osgfsd_s.lib"
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "osgPlugin osgfs - Win32 Release"
# Name "osgPlugin osgfs - Win32 Debug"
# Name "osgPlugin osgfs - Win32 Release Static"
# Name "osgPlugin osgfs - Win32 Debug Static"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\Input.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\Output.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\AlphaFunc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\AnimationPath.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\AutoTransform.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\Billboard.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\BlendFunc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\CameraNode.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\CameraView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\ClearNode.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\ClipNode.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\ClipPlane.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\ClusterCullingCallback.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\ColorMask.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\ColorMatrix.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\ConvexPlanarOccluder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\CoordinateSystemNode.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\CullFace.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\Depth.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\Drawable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\EllipsoidModel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\Fog.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\FragmentProgram.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\FrontFace.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\Geode.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\Geometry.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\Group.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\Image.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\Light.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\LightModel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\LightSource.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\LineStipple.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\LineWidth.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\LOD.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\Material.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\Matrix.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\MatrixTransform.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\Node.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\NodeCallback.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\Object.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\OccluderNode.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\PagedLOD.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\Point.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\PointSprite.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\PolygonMode.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\PolygonOffset.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\PositionAttitudeTransform.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\Program.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\Projection.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\ProxyNode.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\ReaderWriterOSGFS.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\Scissor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\Sequence.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\ShadeModel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\Shader.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\Shape.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\ShapeDrawable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\StateSet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\Stencil.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\Switch.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\TessellationHints.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\TexEnv.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\TexEnvCombine.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\TexEnvFilter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\TexGen.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\TexGenNode.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\TexMat.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\Texture.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\Texture1D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\Texture2D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\Texture3D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\TextureCubeMap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\TextureRectangle.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\Transform.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\Uniform.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\VertexProgram.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\Viewport.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\Input.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\osgPlugins\osgfs\Output.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project

