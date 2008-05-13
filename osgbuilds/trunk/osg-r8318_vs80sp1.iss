; Inno Setup 5.2.0 configuration http://www.innosetup.com/
; for OpenSceneGraph x.x http://openscenegraph.org/
; by Mike Weiblen http://mew.cx/

[Setup]
AppName=OpenSceneGraph
AppVerName=OpenSceneGraph 2.5-r8318 BETA TEST vs8.0sp1 2008-05-13
OutputBaseFilename=osg2.5-r8318_vs80sp1_setup_2008-05-13
VersionInfoTextVersion=2.5-r8318_vs80sp1
VersionInfoVersion=2.5.0.8318
AppPublisher=http://mew.cx/
AppPublisherURL=http://mew.cx/osg/
DefaultDirName={pf}\OpenSceneGraph
DefaultGroupName=OpenSceneGraph
AllowNoIcons=yes
AllowRootDirectory=yes
;LicenseFile=LICENSE.txt
;InfoBeforeFile=win32\PREINSTALL.txt
InfoAfterFile=win32\POSTINSTALL.txt
Compression=lzma
SolidCompression=yes
SetupIconFile=externals\OpenSceneGraph\PlatformSpecifics\Windows\icons\osg.ico
UninstallDisplayIcon={app}\osg.ico
OutputDir=.
TouchDate=none
TouchTime=none


[Messages]
BeveledLabel=Inno Setup


[Tasks]
Name: "setenvars";    Description: "Set OSG environment variables in Registry"; GroupDescription: "Environment variables:"
Name: "desktopicon";  Description: "Create &desktop icons"; GroupDescription: "Additional icons:"
Name: "associateRgb"; Description: "Associate SGI .rgb/.rgba image files with osgviewer"; GroupDescription: "File Associations:"
Name: "associateFlt"; Description: "Associate OpenFlight .flt files with osgviewer"; GroupDescription: "File Associations:"
Name: "associateDae"; Description: "Associate COLLADA .dae documents with osgviewer"; GroupDescription: "File Associations:"


[Registry]
Root: HKLM; Subkey: "SYSTEM\CurrentControlSet\Control\Session Manager\Environment"; ValueType: string; ValueName: "OSG_ROOT"; ValueData: "{app}"; Flags: uninsdeletevalue; Tasks: setenvars
Root: HKLM; Subkey: "SYSTEM\CurrentControlSet\Control\Session Manager\Environment"; ValueType: string; ValueName: "OSG_PATH"; ValueData: "{app}\bin"; Flags: uninsdeletevalue; Tasks: setenvars
Root: HKLM; Subkey: "SYSTEM\CurrentControlSet\Control\Session Manager\Environment"; ValueType: string; ValueName: "OSG_FILE_PATH"; ValueData: "{app}\data;{app}\data\Images;{app}\data\fonts"; Flags: uninsdeletevalue; Tasks: setenvars

Root: HKCR; Subkey: ".osg"; ValueType: string; ValueData: "osgfile"; Flags: uninsdeletevalue createvalueifdoesntexist
Root: HKCR; Subkey: "osgfile"; ValueType: string; ValueData: "OpenSceneGraph text scene"; Flags: uninsdeletevalue
Root: HKCR; Subkey: "osgfile\DefaultIcon"; ValueType: string; ValueData: "{app}\osg.ico,0"
Root: HKCR; Subkey: "osgfile\shell"; ValueType: string; ValueData: "Open"; Flags: uninsdeletekey
Root: HKCR; Subkey: "osgfile\shell\Open\command"; ValueType: string; ValueData: """{app}\bin\osgviewer.exe"" ""%1"""
Root: HKCR; Subkey: "osgfile\shell\Edit\command"; ValueType: string; ValueData: """{pf}\Windows NT\Accessories\wordpad.exe"" ""%1"""
Root: HKCR; Subkey: "osgfile\shell\Convert_to_IveDxt5"; ValueType: string; ValueData: "Convert to IveDxt5"; Flags: uninsdeletekey
Root: HKCR; Subkey: "osgfile\shell\Convert_to_Ive\command"; ValueType: string; ValueData: """{app}\bin\osgconv2ive.bat"" ""%1"""
Root: HKCR; Subkey: "osgfile\shell\Convert_to_IveDxt5"; ValueType: string; ValueData: "Convert to IveDxt5"; Flags: uninsdeletekey
Root: HKCR; Subkey: "osgfile\shell\Convert_to_IveDxt5\command"; ValueType: string; ValueData: """{app}\bin\osgconv2iveDxt5.bat"" ""%1"""

Root: HKCR; Subkey: ".osga"; ValueType: string; ValueData: "osgafile"; Flags: uninsdeletekey
Root: HKCR; Subkey: "osgafile"; ValueType: string; ValueData: "OpenSceneGraph scene archive"; Flags: uninsdeletekey
Root: HKCR; Subkey: "osgafile\DefaultIcon"; ValueType: string; ValueData: "{app}\osg.ico,0"; Flags: uninsdeletevalue
Root: HKCR; Subkey: "osgafile\shell"; ValueType: string; ValueData: "Open"; Flags: uninsdeletekey
Root: HKCR; Subkey: "osgafile\shell\Open\command"; ValueType: string; ValueData: """{app}\bin\osgviewer.exe"""" ""%1"""; Flags: uninsdeletevalue

Root: HKCR; Subkey: ".ive"; ValueType: string; ValueData: "ivefile"; Flags: uninsdeletevalue createvalueifdoesntexist
Root: HKCR; Subkey: "ivefile"; ValueType: string; ValueData: "OpenSceneGraph binary scene"; Flags: uninsdeletevalue
Root: HKCR; Subkey: "ivefile\DefaultIcon"; ValueType: string; ValueData: "{app}\osg.ico,0"
Root: HKCR; Subkey: "ivefile\shell"; ValueType: string; ValueData: "Open"; Flags: uninsdeletekey
Root: HKCR; Subkey: "ivefile\shell\Open\command"; ValueType: string; ValueData: """{app}\bin\osgviewer.exe"" ""%1"""
Root: HKCR; Subkey: "ivefile\shell\Convert_to_Osg"; ValueType: string; ValueData: "Convert to Osg"; Flags: uninsdeletekey
Root: HKCR; Subkey: "ivefile\shell\Convert_to_Osg\command"; ValueType: string; ValueData: """{app}\bin\osgconv2osg.bat"" ""%1"""

Root: HKCR; Subkey: ".flt"; ValueType: string; ValueData: "fltfile"; Flags: uninsdeletevalue; Tasks: associateFlt
Root: HKCR; Subkey: "fltfile"; ValueType: string; ValueData: "OpenFlight model"; Flags: uninsdeletekey; Tasks: associateFlt
Root: HKCR; Subkey: "fltfile\DefaultIcon"; ValueType: string; ValueData: "{app}\osg.ico,0"; Flags: uninsdeletevalue; Tasks: associateFlt
Root: HKCR; Subkey: "fltfile\shell"; ValueType: string; ValueData: "Open"; Flags: uninsdeletekey; Tasks: associateFlt
Root: HKCR; Subkey: "fltfile\shell\Open\command"; ValueType: string; ValueData: """{app}\bin\osgviewer.exe"" ""%1"""; Flags: uninsdeletevalue; Tasks: associateFlt
Root: HKCR; Subkey: "fltfile\shell\Convert_to_Ive"; ValueType: string; ValueData: "Convert to Ive"; Flags: uninsdeletekey; Tasks: associateFlt
Root: HKCR; Subkey: "fltfile\shell\Convert_to_Ive\command"; ValueType: string; ValueData: """{app}\bin\osgconv2iveDxt5.bat"" ""%1"""; Flags: uninsdeletevalue; Tasks: associateFlt
Root: HKCR; Subkey: "fltfile\shell\Convert_to_IveDxt5"; ValueType: string; ValueData: "Convert to IveDxt5"; Flags: uninsdeletekey; Tasks: associateFlt
Root: HKCR; Subkey: "fltfile\shell\Convert_to_IveDxt5\command"; ValueType: string; ValueData: """{app}\bin\osgconv2ive.bat"" ""%1"""; Flags: uninsdeletevalue; Tasks: associateFlt
Root: HKCR; Subkey: "fltfile\shell\Convert_to_Osg"; ValueType: string; ValueData: "Convert to Osg"; Flags: uninsdeletekey; Tasks: associateFlt
Root: HKCR; Subkey: "fltfile\shell\Convert_to_Osg\command"; ValueType: string; ValueData: """{app}\bin\osgconv2osg.bat"" ""%1"""; Flags: uninsdeletevalue; Tasks: associateFlt

Root: HKCR; Subkey: ".rgb"; ValueType: string; ValueData: "rgbfile"; Flags: uninsdeletevalue; Tasks: associateRgb
Root: HKCR; Subkey: "rgbfile"; ValueType: string; ValueData: "SGI RGB image"; Flags: uninsdeletekey; Tasks: associateRgb
Root: HKCR; Subkey: "rgbfile\DefaultIcon"; ValueType: string; ValueData: "{app}\osg.ico,0"; Flags: uninsdeletevalue; Tasks: associateRgb
Root: HKCR; Subkey: "rgbfile\shell"; ValueType: string; ValueData: "Open"; Flags: uninsdeletekey; Tasks: associateRgb
Root: HKCR; Subkey: "rgbfile\shell\Open\command"; ValueType: string; ValueData: """{app}\bin\osgviewer.exe"" --image ""%1"""; Flags: uninsdeletevalue; Tasks: associateRgb
Root: HKCR; Subkey: ".rgba"; ValueType: string; ValueData: "rgbafile"; Flags: uninsdeletekey; Tasks: associateRgb
Root: HKCR; Subkey: "rgbafile"; ValueType: string; ValueData: "SGI RGBA image"; Flags: uninsdeletekey; Tasks: associateRgb
Root: HKCR; Subkey: "rgbafile\DefaultIcon"; ValueType: string; ValueData: "{app}\osg.ico,0"; Flags: uninsdeletevalue; Tasks: associateRgb
Root: HKCR; Subkey: "rgbafile\shell"; ValueType: string; ValueData: "Open"; Flags: uninsdeletekey; Tasks: associateRgb
Root: HKCR; Subkey: "rgbafile\shell\Open\command"; ValueType: string; ValueData: """{app}\bin\osgviewer.exe"" --image ""%1"""; Flags: uninsdeletevalue; Tasks: associateRgb

Root: HKCR; Subkey: ".dae"; ValueType: string; ValueData: "daefile"; Flags: uninsdeletekey; Tasks: associateDae
Root: HKCR; Subkey: "daefile"; ValueType: string; ValueData: "COLLADA document"; Flags: uninsdeletekey; Tasks: associateDae
Root: HKCR; Subkey: "daefile\DefaultIcon"; ValueType: string; ValueData: "{app}\osg.ico,0"; Flags: uninsdeletevalue; Tasks: associateDae
Root: HKCR; Subkey: "daefile\shell"; ValueType: string; ValueData: "Open"; Flags: uninsdeletekey; Tasks: associateDae
Root: HKCR; Subkey: "daefile\shell\Open\command"; ValueType: string; ValueData: """{app}\bin\osgviewer.exe"" ""%1"""; Flags: uninsdeletevalue; Tasks: associateDae
Root: HKCR; Subkey: "daefile\shell\Edit\command"; ValueType: string; ValueData: """{pf}\Windows NT\Accessories\wordpad.exe"" ""%1"""; Flags: uninsdeletevalue; Tasks: associateDae
Root: HKCR; Subkey: "daefile\shell\Convert_to_Ive"; ValueType: string; ValueData: "Convert to Ive"; Flags: uninsdeletekey; Tasks: associateDae
Root: HKCR; Subkey: "daefile\shell\Convert_to_Ive\command"; ValueType: string; ValueData: """{app}\bin\conv2ive.bat"" ""%1"""; Flags: uninsdeletevalue; Tasks: associateDae
Root: HKCR; Subkey: "daefile\shell\Convert_to_IveDxt5"; ValueType: string; ValueData: "Convert to IveDxt5"; Flags: uninsdeletekey; Tasks: associateDae
Root: HKCR; Subkey: "daefile\shell\Convert_to_IveDxt5\command"; ValueType: string; ValueData: """{app}\bin\conv2iveDxt5.bat"" ""%1"""; Flags: uninsdeletevalue; Tasks: associateDae


[Files]
; NOTE: Don't use "Flags: ignoreversion" on any shared system files
Source: "win32\README_vs80.txt"; DestDir: "{app}"; Flags: isreadme ignoreversion
Source: "externals\OpenSceneGraph\PlatformSpecifics\Windows\icons\osg.ico"; DestDir: "{app}"; Flags: ignoreversion
Source: "osgsrc.zip"; DestDir: "{app}"; Flags: ignoreversion

; URLs
Source: "win32\URLs\*"; DestDir: "{app}\URLs"; Flags: ignoreversion recursesubdirs


; doc
Source: "externals\OpenSceneGraph\AUTHORS.txt";    DestDir: "{app}\doc\OpenSceneGraph"; Flags: ignoreversion
Source: "externals\OpenSceneGraph\LICENSE.txt";    DestDir: "{app}\doc\OpenSceneGraph"; Flags: ignoreversion
Source: "externals\OpenSceneGraph\NEWS.txt";       DestDir: "{app}\doc\OpenSceneGraph"; Flags: ignoreversion
Source: "externals\OpenSceneGraph\README.txt";     DestDir: "{app}\doc\OpenSceneGraph"; Flags: ignoreversion
Source: "externals\osgToy\README_osgToy.txt";      DestDir: "{app}\doc\osgToy"; Flags: ignoreversion
Source: "externals\osgToy\3Dlabs_license.txt";     DestDir: "{app}\doc\osgToy"; Flags: ignoreversion
Source: "externals\3rdParty\README_3rdParty.txt";  DestDir: "{app}\doc"; Flags: ignoreversion


; include
Source: "externals\3rdParty\include\*"; Excludes: ".svn"; DestDir: "{app}\include"; Flags: ignoreversion recursesubdirs
Source: "externals\OpenSceneGraph\include\*"; Excludes: ".svn"; DestDir: "{app}\include"; Flags: ignoreversion recursesubdirs
Source: "externals\osgToy\include\*"; Excludes: ".svn"; DestDir: "{app}\include"; Flags: ignoreversion recursesubdirs
Source: "externals\osgVRPN\include\*"; Excludes: ".svn"; DestDir: "{app}\include"; Flags: ignoreversion recursesubdirs


; bin
Source: "win32\bin\*"; DestDir: "{app}\bin"; Flags: ignoreversion recursesubdirs
Source: "externals\3rdParty\bin\*"; DestDir: "{app}\bin"; Flags: ignoreversion recursesubdirs
Source: "_INSTALL_osg_Release\bin\*"; DestDir: "{app}\bin"; Flags: ignoreversion recursesubdirs
Source: "_INSTALL_osg_Release\share\OpenSceneGraph\bin\*"; DestDir: "{app}\bin"; Flags: ignoreversion recursesubdirs
;Source: "osgToy\bin\win32\*.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
;Source: "osgVRPN\bin\*.dll"; DestDir: "{app}\bin"; Flags: ignoreversion


; lib
Source: "externals\3rdParty\lib\*"; Excludes: ".svn"; DestDir: "{app}\lib"; Flags: ignoreversion recursesubdirs
Source: "_INSTALL_osg_Release\lib\*"; Excludes: ".svn"; DestDir: "{app}\lib"; Flags: ignoreversion recursesubdirs
;Source: "osgToy\lib\win32\*.lib"; DestDir: "{app}\lib"; Flags: ignoreversion
;Source: "osgVRPN\lib\*.lib"; DestDir: "{app}\lib"; Flags: ignoreversion


; data
Source: "externals\3rdParty\data\*"; Excludes: ".svn"; DestDir: "{app}\data"; Flags: ignoreversion recursesubdirs
Source: "externals\OpenSceneGraph-Data\*"; Excludes: ".svn"; DestDir: "{app}\data"; Flags: ignoreversion recursesubdirs

Source: "externals\osgToy-Data\cubemap_test\*"; Excludes: ".svn"; DestDir: "{app}\data\cubemap_test"; Flags: ignoreversion recursesubdirs
Source: "externals\osgToy-Data\Images\colorbars.png"; Excludes: ".svn"; DestDir: "{app}\data\Images"; Flags: ignoreversion recursesubdirs
Source: "externals\osgToy-Data\Images\stars2048_yalecyl1.png"; Excludes: ".svn"; DestDir: "{app}\data\Images"; Flags: ignoreversion recursesubdirs
Source: "externals\osgToy-Data\Images\stars2048_yalecyl1.txt"; Excludes: ".svn"; DestDir: "{app}\data\Images"; Flags: ignoreversion recursesubdirs
Source: "externals\osgToy-Data\Images\verasansmono.png"; Excludes: ".svn"; DestDir: "{app}\data\Images"; Flags: ignoreversion recursesubdirs
Source: "externals\osgToy-Data\Images\R64.png"; Excludes: ".svn"; DestDir: "{app}\data\Images"; Flags: ignoreversion recursesubdirs
Source: "externals\osgToy-Data\Images\X32.png"; Excludes: ".svn"; DestDir: "{app}\data\Images"; Flags: ignoreversion recursesubdirs
Source: "externals\osgToy-Data\shaders\drawtext.frag"; Excludes: ".svn"; DestDir: "{app}\data\shaders"; Flags: ignoreversion recursesubdirs
Source: "externals\osgToy-Data\shaders\drawtext4.frag"; Excludes: ".svn"; DestDir: "{app}\data\shaders"; Flags: ignoreversion recursesubdirs
Source: "externals\osgToy-Data\4shapes.osg"; Excludes: ".svn"; DestDir: "{app}\data"; Flags: ignoreversion recursesubdirs
Source: "externals\osgToy-Data\box_grn.osg"; Excludes: ".svn"; DestDir: "{app}\data"; Flags: ignoreversion recursesubdirs
Source: "externals\osgToy-Data\colorbars.osg"; Excludes: ".svn"; DestDir: "{app}\data"; Flags: ignoreversion recursesubdirs
Source: "externals\osgToy-Data\compactdisc.osg"; Excludes: ".svn"; DestDir: "{app}\data"; Flags: ignoreversion recursesubdirs
Source: "externals\osgToy-Data\cone_red.osg"; Excludes: ".svn"; DestDir: "{app}\data"; Flags: ignoreversion recursesubdirs
Source: "externals\osgToy-Data\cylinder_blu.osg"; Excludes: ".svn"; DestDir: "{app}\data"; Flags: ignoreversion recursesubdirs
Source: "externals\osgToy-Data\dodecahedron.osg"; Excludes: ".svn"; DestDir: "{app}\data"; Flags: ignoreversion recursesubdirs
Source: "externals\osgToy-Data\glsl_dataflag.osg"; Excludes: ".svn"; DestDir: "{app}\data"; Flags: ignoreversion recursesubdirs
Source: "externals\osgToy-Data\glsllint_errors.osg"; Excludes: ".svn"; DestDir: "{app}\data"; Flags: ignoreversion recursesubdirs
Source: "externals\osgToy-Data\grid_xy.osg"; Excludes: ".svn"; DestDir: "{app}\data"; Flags: ignoreversion recursesubdirs
Source: "externals\osgToy-Data\grid_xz.osg"; Excludes: ".svn"; DestDir: "{app}\data"; Flags: ignoreversion recursesubdirs
Source: "externals\osgToy-Data\grid_yz.osg"; Excludes: ".svn"; DestDir: "{app}\data"; Flags: ignoreversion recursesubdirs
Source: "externals\osgToy-Data\icosahedron.osg"; Excludes: ".svn"; DestDir: "{app}\data"; Flags: ignoreversion recursesubdirs
Source: "externals\osgToy-Data\no_normals.osg"; Excludes: ".svn"; DestDir: "{app}\data"; Flags: ignoreversion recursesubdirs
Source: "externals\osgToy-Data\octahedron.osg"; Excludes: ".svn"; DestDir: "{app}\data"; Flags: ignoreversion recursesubdirs
Source: "externals\osgToy-Data\octostrip.osg"; Excludes: ".svn"; DestDir: "{app}\data"; Flags: ignoreversion recursesubdirs
Source: "externals\osgToy-Data\penrose_triangle.osg"; Excludes: ".svn"; DestDir: "{app}\data"; Flags: ignoreversion recursesubdirs
Source: "externals\osgToy-Data\rhombic_dodecahedron.osg"; Excludes: ".svn"; DestDir: "{app}\data"; Flags: ignoreversion recursesubdirs
Source: "externals\osgToy-Data\ss3d_5star.lua"; Excludes: ".svn"; DestDir: "{app}\data"; Flags: ignoreversion recursesubdirs
Source: "externals\osgToy-Data\ss3d_cube.lua"; Excludes: ".svn"; DestDir: "{app}\data"; Flags: ignoreversion recursesubdirs
Source: "externals\osgToy-Data\ss3d_default.lua"; Excludes: ".svn"; DestDir: "{app}\data"; Flags: ignoreversion recursesubdirs
Source: "externals\osgToy-Data\ss3d_trillium.lua"; Excludes: ".svn"; DestDir: "{app}\data"; Flags: ignoreversion recursesubdirs
Source: "externals\osgToy-Data\ss3d_umbrella.lua"; Excludes: ".svn"; DestDir: "{app}\data"; Flags: ignoreversion recursesubdirs
Source: "externals\osgToy-Data\ss3d_urchin.lua"; Excludes: ".svn"; DestDir: "{app}\data"; Flags: ignoreversion recursesubdirs
Source: "externals\osgToy-Data\starsphere.osg"; Excludes: ".svn"; DestDir: "{app}\data"; Flags: ignoreversion recursesubdirs
Source: "externals\osgToy-Data\tetrahedron.osg"; Excludes: ".svn"; DestDir: "{app}\data"; Flags: ignoreversion recursesubdirs
Source: "externals\osgToy-Data\tet6.osg"; Excludes: ".svn"; DestDir: "{app}\data"; Flags: ignoreversion recursesubdirs
Source: "externals\osgToy-Data\tet7.osg"; Excludes: ".svn"; DestDir: "{app}\data"; Flags: ignoreversion recursesubdirs
Source: "externals\osgToy-Data\tet8.osg"; Excludes: ".svn"; DestDir: "{app}\data"; Flags: ignoreversion recursesubdirs
Source: "externals\osgToy-Data\xyz1.osg"; Excludes: ".svn"; DestDir: "{app}\data"; Flags: ignoreversion recursesubdirs
Source: "externals\osgToy-Data\xyz3.osg"; Excludes: ".svn"; DestDir: "{app}\data"; Flags: ignoreversion recursesubdirs
Source: "externals\osgToy-Data\xyz6.osg"; Excludes: ".svn"; DestDir: "{app}\data"; Flags: ignoreversion recursesubdirs
Source: "externals\osgToy-Data\xyz100.osg"; Excludes: ".svn"; DestDir: "{app}\data"; Flags: ignoreversion recursesubdirs


[Icons]
Name: "{userdesktop}\osgLogo";  Filename: "{app}\bin\osglogo.exe";  Parameters: "--version"; IconFilename: "{app}\osg.ico"; WorkingDir: "{app}\bin"; Tasks: desktopicon
Name: "{userdesktop}\osgShell"; Filename: "{app}\bin\osgShell.bat"; IconFilename: "{app}\osg.ico"; WorkingDir: "%OSG_ROOT%\bin"; Tasks: desktopicon

Name: "{group}\Uninstall OpenSceneGraph 2.5-r8318 BETA TEST vs8.0sp1 2008-05-13" ; Filename: "{uninstallexe}"
Name: "{group}\View README"; Filename: "{app}\README_vs80.txt";
Name: "{group}\View OSG Program Files folder"; Filename: "{app}"
Name: "{group}\View OSG Start Menu folder"; Filename: "{group}"
Name: "{group}\osgShell"; Filename: "{app}\bin\osgShell.bat"; IconFilename: "{app}\osg.ico"; WorkingDir: "%OSG_ROOT%\bin"

Name: "{group}\URLs\Browse SVN"; Filename: "{app}\URLs\OSG svn browser.URL"
Name: "{group}\URLs\mew.cx"; Filename: "{app}\URLs\mew.cx OSG website.URL"
Name: "{group}\URLs\OpenSceneGraph"; Filename: "{app}\URLs\OSG website.URL"
Name: "{group}\URLs\osgToy"; Filename: "{app}\URLs\osgToy website.URL"
Name: "{group}\URLs\osg-users mailing list"; Filename: "{app}\URLs\osg-users mailing list.URL"
Name: "{group}\URLs\osg-submissions mailing list"; Filename: "{app}\URLs\osg-submissions mailing list.URL"
Name: "{group}\URLs\VRPN"; Filename: "{app}\URLs\VRPN website.URL"

Name: "{group}\Demos\osganimate"; Filename: "{app}\bin\osganimate.exe"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgautotransform"; Filename: "{app}\bin\osgautotransform.exe"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgbillboard"; Filename: "{app}\bin\osgbillboard.exe"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgblendequation"; Filename: "{app}\bin\osgblendequation.exe"; Parameters: "cessnafire.osg"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgclip"; Filename: "{app}\bin\osgclip.exe"; Parameters: "cow.osg"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgcompositeviewer"; Filename: "{app}\bin\osgcompositeviewer.exe"; Parameters: "cow.osg"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgcubemap"; Filename: "{app}\bin\osgcubemap.exe"; Parameters: "cessna.osg"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgdelaunay"; Filename: "{app}\bin\osgdelaunay.exe"; WorkingDir: "{app}\bin"
;Name: "{group}\Demos\osgdepthshadow"; Filename: "{app}\bin\osgdepthshadow.exe"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgdepthpeeling"; Filename: "{app}\bin\osgdepthpeeling.exe"; Parameters: "cow.osg"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgdistortion"; Filename: "{app}\bin\osgdistortion.exe"; Parameters: "cow.osg"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgfadetext"; Filename: "{app}\bin\osgfadetext.exe"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgforest"; Filename: "{app}\bin\osgforest.exe"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgfxbrowser"; Filename: "{app}\bin\osgfxbrowser.exe"; Parameters: "dumptruck.osg"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osggeometry"; Filename: "{app}\bin\osggeometry.exe"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osghangglide"; Filename: "{app}\bin\osghangglide.exe"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osghud"; Filename: "{app}\bin\osghud.exe"; Parameters: "dumptruck.osg"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgimpostor"; Filename: "{app}\bin\osgimpostor.exe"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgkeyboard"; Filename: "{app}\bin\osgkeyboard.exe"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgkeyboardmouse"; Filename: "{app}\bin\osgkeyboardmouse.exe"; Parameters: "spaceship.osg"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osglauncher"; Filename: "{app}\bin\osglauncher.exe"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osglight"; Filename: "{app}\bin\osglight.exe"; Parameters: "glider.osg"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osglightpoint"; Filename: "{app}\bin\osglightpoint.exe"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osglogicop"; Filename: "{app}\bin\osglogicop.exe"; Parameters: "glider.osg"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osglogo"; Filename: "{app}\bin\osglogo.exe"; Parameters: "--version"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgmotionblur"; Filename: "{app}\bin\osgmotionblur.exe"; Parameters: "cow.osg"; WorkingDir: "{app}\bin"
;Name: "{group}\Demos\osgmultiplecameras"; Filename: "{app}\bin\osgmultiplecameras.exe"; Parameters: "cessna.osg"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgmultitexture"; Filename: "{app}\bin\osgmultitexture.exe"; Parameters: "cessnafire.osg"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgoccluder"; Filename: "{app}\bin\osgoccluder.exe"; Parameters: "glider.osg"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgparametric"; Filename: "{app}\bin\osgparametric.exe"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgparticle"; Filename: "{app}\bin\osgparticle.exe"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgparticleeffects"; Filename: "{app}\bin\osgparticleeffects.exe"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgpick"; Filename: "{app}\bin\osgpick.exe"; Parameters: "fountain.osg"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgplanets"; Filename: "{app}\bin\osgplanets.exe"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgpointsprite"; Filename: "{app}\bin\osgpointsprite.exe"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgprecipitation rain"; Filename: "{app}\bin\osgprecipitation.exe"; Parameters: "--rain 0.5 lz.osg"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgprecipitation snow"; Filename: "{app}\bin\osgprecipitation.exe"; Parameters: "--snow 0.5 lz.osg"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgprerender"; Filename: "{app}\bin\osgprerender.exe"; Parameters: "dumptruck.osg"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgprerendercubemap"; Filename: "{app}\bin\osgprerendercubemap.exe"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgreflect"; Filename: "{app}\bin\osgreflect.exe"; Parameters: "cessna.osg"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgscalarbar"; Filename: "{app}\bin\osgscalarbar.exe"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgscribe"; Filename: "{app}\bin\osgscribe.exe"; Parameters: "cow.osg"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgsequence"; Filename: "{app}\bin\osgsequence.exe"; Parameters: "cow.osg dumptruck.osg cessna.osg glider.osg"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgshaders"; Filename: "{app}\bin\osgshaders.exe"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgshaderterrain"; Filename: "{app}\bin\osgshaderterrain.exe"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgshadow"; Filename: "{app}\bin\osgshadow.exe"; Parameters: "cessna.osg"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgshape"; Filename: "{app}\bin\osgshape.exe"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgsimplifier"; Filename: "{app}\bin\osgsimplifier.exe"; Parameters: "--ratio 0.1 --max-error 0.05 cessna.osg"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgsimulation"; Filename: "{app}\bin\osgsimulation.exe"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgspacewarp"; Filename: "{app}\bin\osgspacewarp.exe"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgspheresegment"; Filename: "{app}\bin\osgspheresegment.exe"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgspotlight"; Filename: "{app}\bin\osgspotlight.exe"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgstereoimage"; Filename: "{app}\bin\osgstereoimage.exe"; Parameters: "Images/dog_left_eye.jpg Images/dog_right_eye.jpg"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgteapot"; Filename: "{app}\bin\osgteapot.exe"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgtessellate"; Filename: "{app}\bin\osgtessellate.exe"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgtext"; Filename: "{app}\bin\osgtext.exe"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgtexture1D"; Filename: "{app}\bin\osgtexture1D.exe"; Parameters: "dumptruck.osg"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgtexture2D"; Filename: "{app}\bin\osgtexture2D.exe"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgtexture3D"; Filename: "{app}\bin\osgtexture3D.exe"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgtexturerectangle"; Filename: "{app}\bin\osgtexturerectangle.exe"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgthirdpersonview"; Filename: "{app}\bin\osgthirdpersonview.exe"; Parameters: "cessnafire.osg"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgvertexprogram"; Filename: "{app}\bin\osgvertexprogram.exe"; WorkingDir: "{app}\bin"
;Name: "{group}\Demos\osgviewer 2cows+logo"; Filename: "{app}\bin\osgviewer.exe"; Parameters: "cow.osg cow.osg.3.scale.0,0,225.rot.15,0,0.trans osg64.logo"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgviewer cow"; Filename: "{app}\bin\osgviewer.exe"; Parameters: "cow.osg"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgviewer cubemapped torus"; Filename: "{app}\bin\osgviewer.exe"; Parameters: "cube_mapped_torus.osg"; WorkingDir: "{app}\bin"
;Name: "{group}\Demos\osgviewer earth+moon globes"; Filename: "{app}\bin\osgviewer.exe"; Parameters: "Images/land_shallow_topo_2048.jpg.12700.globe SolarSystem/moon256128.jpg.3500.globe.38400,0,0.trans"; WorkingDir: "{app}\bin"
;Name: "{group}\Demos\osgviewer earth_test (via HTTP)"; Filename: "{app}\bin\osgviewer.exe"; Parameters: "http://www.openscenegraph.net/data/earth_test/model.ive"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgviewer osgcool"; Filename: "{app}\bin\osgviewer.exe"; Parameters: "osgcool.osg"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgviewer stereo cessna"; Filename: "{app}\bin\osgviewer.exe"; Parameters: "--stereo cessna.osg"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgviewerGLUT"; Filename: "{app}\bin\osgviewerGLUT.exe"; Parameters: "spaceship.osg"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgviewerMFC"; Filename: "{app}\bin\osgviewerMFC.exe"; WorkingDir: "{app}\data"
Name: "{group}\Demos\osgvolume"; Filename: "{app}\bin\osgvolume.exe"; Parameters: "Images/osg256.png"; WorkingDir: "{app}\bin"
Name: "{group}\Demos\osgwindows"; Filename: "{app}\bin\osgwindows.exe"; Parameters: "glider.osg"; WorkingDir: "{app}\bin"

; vim: set sw=4 ts=8 et ic ai:
