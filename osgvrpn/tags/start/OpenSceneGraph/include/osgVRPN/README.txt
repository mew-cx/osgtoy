osgVRPN/README.txt - Mike Weiblen mew@mew.cx - 2003-11-30

A library to integrate the Virtual Reality Peripheral Network with the
OpenSceneGraph 3D graphics system.  VRPN is a library (with sample client
and server applications) to provide network-transparent access to a wide
variety of VR devices.

osgVRPN is in an early stage of development, but seems functional enough
to share with others.  The one VRPN device currently supported is the
"tracker":
- osgVRPN::TrackerManipulator allows a camera to be positioned using
  a tracker.
- osgVRPN::TrackerTransform allows a scenegraph Node to be positioned
  using a tracker.

I developed osgVRPN using a Spaceball 2003 6-DOF controller mapped as a 
vrpn_Tracker_AnalogFly tracker emulator; see osgVRPNviewer/README.txt
for details.


CREDITS & THANKS

* For VRPN: The NIH National Research Resource in Molecular Graphics and
  Microscopy at the University of North Carolina at Chapel Hill

* For OSG: Robert Osfield and the OSG community


REFERENCES

http://www.openscenegraph.org/
http://www.vrpn.org/ or http://www.cs.unc.edu/Research/vrpn/
http://www.mew.cx/osg/

#EOF
