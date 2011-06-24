osgVRPNviewer/README.txt - Mike Weiblen mew@mew.cx
$Id$

A simple application to demonstrate osgVRPN classes.

For this app to be interesting, you must have a vrpn_server running.
The RUN_vrpn_server.bat script runs my default development configuration.
See the VRPN website for details on configuring/running a VRPN server.

To run the app, simply invoke "osgVRPNviewer" with no arguments.

In the default configuration, the tracker is attached to an
osgVRPN::TrackerManipulator, so the camera viewpoint is controlled by
the tracker.

The app may be configured to attach the tracker to an osgVRPN::TrackerTransform
on the central cone, so that cone is controlled by the tracker (see the gUse*
booleans near the top of osgVRPNviewer.cpp)


CREDITS & THANKS

* For VRPN: The NIH National Research Resource in Molecular Graphics and
  Microscopy at the University of North Carolina at Chapel Hill

* For OSG: Robert Osfield and the OSG community


REFERENCES

http://openscenegraph.org/
http://vrpn.org/ or http://www.cs.unc.edu/Research/vrpn/
http://mew.cx/osg/
http://sourceforge.net/projects/osgtoy   <-- CVS tree is here

#EOF
