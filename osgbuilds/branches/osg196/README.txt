$Id$

1) Copy the "SETENV.bat.tmpl" template to SETENV.bat
2) Make any necessary local adjustments to SETENV.bat
3) Launch the RUN_CMAKE.bat script to create the VS project files.
4) Launch the RUN_VISUALSTUDIO.bat script to start the VisualStudio IDE.
   Do not launch VS directly using the .sln; the script must set up several
   environment variables.
5) Or launch the RUN_FULL_BUILD.bat script to build & install the whole thing.


places to change project name:
1) svn:external
2) SETENV.bat
3) CMakeLists.txt

