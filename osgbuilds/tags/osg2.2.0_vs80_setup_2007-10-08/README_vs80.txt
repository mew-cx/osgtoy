README_vs71.txt - mew 2007-10-08
$Id$

This is a development package for the OpenSceneGraph.

This package is intended for use with VisualStudio v8.0 2005 SP1, although
all the workspaces and projects are expressed in the older but very flexible
VC6 .dsw/.dsp project format.  When loaded into VS7.1, at startup you will be
asked to convert the projects to the newer format; say yes.

Enclosed are 2 workspace .dsw files, with a different sets of projects:
  OpenSceneGraph.dsw - OSG's original workspace file
  OpenSceneGraph+xtra.dsw - the original workspace + my extras (eg everything)

(Note: the "xtra" workspace was created by appending my *.dswx "dsw extension"
files to the original OpenSceneGraph.dsw file, using:
cat OpenSceneGraph.dsw *.dswx > OpenSceneGraph+xtra.dsw)

However, rather than loading those workspaces directly, I recommend using the
corresponding .bat scripts, which will configure essential environment
variables, then launch VS7.1:
  RUN_vs71_OSG.bat
  RUN_vs71_OSG+xtra.bat

Please review and adjust SETENV.bat for your prefered environment.

#EOF
