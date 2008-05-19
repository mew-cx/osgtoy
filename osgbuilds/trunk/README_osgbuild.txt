edited from a thread begining at
http://comments.gmane.org/gmane.comp.graphics.openscenegraph.user/27567

Hi,

As a datapoint, here is my recipe for building OSG 2.4.0 on
VS2005sp1/WinXPsp2 that I use for the win32 binary installer.
Motivated by my own dreadful memory, my goal is to make the whole
process as foolproof and turnkey as possible...

0) svn checkout --ignore-externals https://osgtoy.svn.sourceforge.net/svnroot/osgtoy/osgbuilds/tags/osg2.4.0_vs80sp1_setup_2008-04-28 %MY_OSG_TREE%
(or trunk if you want my raw development)
That is my SVN "build configuration" that points to all the necessary
components via svn:external references to other project's tags.  
This tree is only ~98KB.  But thanks to the magic of svn:external,
it contains all information about how to locate the other project
subsystems relative to each other.

(fyi, this is why I really like using SVN to distribute e.g. the
3rdParty prebuilts, rather than having to coordinate the unpacking of
.zip snapshots.  Once you embrace svn:external, .zip snapshots are just
 a pain :-)

1) cd %MY_OSG_TREE%; svn update
Go into the build tree, and a real "svn update" will populate all
the external project trees.

2) edit %MY_OSG_TREE%\SETENV.bat to your liking
You should review and decide if/what needs adjusting.  
Note that this file specifies:
- Type of VisualStudio build (Release v. Debug)
- location of CMake product directories (build & install)
(NB SETENV is a centralized container for specifying common environment
variable definitions.  It is never executed by itself; rather it is
CALLed by other .bat files to establish the common global environment)

{{ Q: does SETENV.bat really require the .bat extension??? }}

3) run %MY_OSG_TREE%\x1000_CMAKE.bat
That sets my preference overrides for some CMake defaults then runs
CMake (in non-GUI mode) to create the VS projects.  You should review
and decide if/what needs adjusting.

4) run %MY_OSG_TREE%\x3000_VSIDE.bat
That launches VisualStudio 2005, with the OSG envars initialized for
ease of debugging, using the just-generated project files.

5) build as usual.

Following these steps, I get no errors or significant warnings.  Any
tweaks are preserved in the three .bat scripts mentioned.  My goal is
(and in my experience) it Just Works.

cheers
-- mew

PS Yep, I will capture this is a README

-- 
Mike Weiblen -- Austin Texas USA -- http://mew.cx/

REM vim: set sw=4 ts=8 et ic ai:
