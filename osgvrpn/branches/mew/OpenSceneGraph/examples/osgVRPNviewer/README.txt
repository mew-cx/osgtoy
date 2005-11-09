osgVRPNviewer/README.txt - Mike Weiblen mew@mew.cx - 2003-11-30

A simple application to demonstrate osgVRPN.

Before running the app, you must have a vrpn_server running; see the
VRPN website for details on building and running the VRPN server.
In this directory is the "vrpn.cfg" server configuration file I used for
development/testing.  I developed osgVRPN using a Spaceball 2003 6-DOF
controller mapped as a vrpn_Tracker_AnalogFly tracker emulator.

To run the app, simply invoke "osgVRPNviewer" with no arguments.
The scene consists of two pieces of geometry: a plane and a cone.

In the default configuration, the tracker is attached to an
osgVRPN::TrackerManipulator, so the viewpoint of the scene (both the cone
and the plane) is controlled by the tracker.

The app may be reconfigured (see #ifdef TRACKER_ON_NODE) to attach
the tracker to an osgVRPN::TrackerTransform on the cone's subgraph,
so the cone may be moved independently of the plane.


CREDITS & THANKS

* For VRPN: The NIH National Research Resource in Molecular Graphics and
  Microscopy at the University of North Carolina at Chapel Hill

* For OSG: Robert Osfield and the OSG community


REFERENCES

http://www.openscenegraph.org/
http://www.vrpn.org/ or http://www.cs.unc.edu/Research/vrpn/
http://www.mew.cx/osg/

#EOF
