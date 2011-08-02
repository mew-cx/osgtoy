#! /bin/sh -x

BINDIR=/home/weiblenm/projects/3rdParty/bin

#${BINDIR}/vrpn_print_devices Spaceball0@localhost
#${BINDIR}/vrpn_print_devices Tracker0@localhost
#osgviewer cessna.osg cow.osg.Tracker0@localhost.tracker
#osgVRPNviewer 0.osg 

CFG="./spaceball4000flx_serial.cfg"

sudo ${BINDIR}/vrpn_server -v -millisleep 1 -f ${CFG}

