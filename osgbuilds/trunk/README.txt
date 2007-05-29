$Id$

1) Copy the "SETENV.bat.tmpl" template to SETENV.bat
2) Make any desired local adjustments to SETENV.bat
3) Run the RUN_CMAKE.bat script to create project files.
4) Run the RUN_VISUALSTUDIO.bat script to start VisualStudio.
   Do not launch VS directly using the .sln, as the script must setup
   the environment variables.

places to change project name:
1) svn:external
2) SETENV.bat
3) CMakeLists.txt
