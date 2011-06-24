/* file:        applications/superviewer/VrpnManip.cpp
 * author:      Mike Weiblen
 * copyright:   (C) 2005-2006 Michael Weiblen http://mew.cx/
 * license:     OpenSceneGraph Public License (OSGPL)
 * $Id$
*/

#include <osg/Vec3>
#include <osgProducer/Viewer>

#include <osgVRPN/Analog.h>
#include <osgVRPN/Button.h>
#include <osgVRPN/TrackerBase.h>
#include <osgVRPN/AnalogTracker.h>
#include <osgVRPN/Tracker.h>
#include <osgVRPN/TrackerManipulator.h>

///////////////////////////////////////////////////////////////////////////

static bool gUseAnalogTracker(false);

static osgVRPN::TrackerBase* myTrackerFactory( osgProducer::Viewer* viewer )
{
    if( ! gUseAnalogTracker )
        return new osgVRPN::Tracker( "Tracker0@localhost" );

    const char* deviceName( "Spaceball0@localhost" );
    osgVRPN::AnalogTracker* trk( new osgVRPN::AnalogTracker() );
    trk->setViewer( viewer );
    trk->setAnalogDevice( new osgVRPN::Analog( deviceName ) );
    trk->setTranslateChannelX(0);
    trk->setTranslateChannelY(1);
    trk->setTranslateChannelZ(2);
    trk->setRotateChannelX(3);
    trk->setRotateChannelY(4);
    trk->setRotateChannelZ(5);
    trk->setTranslationScale(osg::Vec3(50,50,50));
    trk->setRotationScale(osg::Vec3(5,5,5));
    trk->setButtonDevice( new osgVRPN::Button( deviceName ) );
    trk->setResetButton(0);
    return trk;
}

///////////////////////////////////////////////////////////////////////////
// TODO this VRPN device stuff needs to be easily configurable,
// say with a lua configuration file loaded at runtime.

void addVrpnManip( osgProducer::Viewer& viewer )
{
    osgVRPN::TrackerBase* tracker( myTrackerFactory( 0 /*&viewer*/ ) );

    osgVRPN::TrackerManipulator* manip( new osgVRPN::TrackerManipulator(tracker) );
    unsigned int pos( viewer.addCameraManipulator(manip) );
    viewer.selectCameraManipulator( pos );
}

// vim: set sw=4 ts=8 et ic ai:
