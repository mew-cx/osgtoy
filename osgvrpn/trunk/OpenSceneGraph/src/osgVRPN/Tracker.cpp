/* file:        src/osgVRPN/Tracker.cpp
 * author:      Mike Weiblen mew@mew.cx
 * copyright:   (C) 2003-2006 Michael Weiblen
 * license:     OpenSceneGraph Public License (OSGPL)
 * $Id: Tracker.cpp,v 1.5 2006/07/01 20:48:52 mew Exp $
*/

#include <osg/Notify>
#include <osgVRPN/Tracker.h>

#define VRPN_CLIENT_ONLY
#include "vrpn_Tracker.h"

using namespace osgVRPN;

///////////////////////////////////////////////////////////////////////////

Tracker::Tracker( const char* deviceName ) :
        _vrpnTracker( new vrpn_Tracker_Remote(deviceName) )
{
    _vrpnTracker->register_change_handler( this, s_ChangeHandler );
}

Tracker::~Tracker()
{
    _vrpnTracker->unregister_change_handler( this, s_ChangeHandler );
    delete _vrpnTracker;
}

///////////////////////////////////////////////////////////////////////////

osg::Matrixd Tracker::getMatrix() const
{
    return osg::Matrixd::rotate( _rotation ) *
            osg::Matrixd::translate( _position );
}

osg::Matrixd Tracker::getInverseMatrix() const
{
    return osg::Matrixd::translate( -_position ) *
            osg::Matrixd::rotate( _rotation.inverse() );
}

///////////////////////////////////////////////////////////////////////////
// Interface to the VRPN message dispatch:
// update() is to be called "often" (e.g.: every frame) to receive messages
// from the VRPN server.
// For each message received, VRPN will invoke the s_ChangeHandler() callback.

bool Tracker::update()
{
    _updateReceivedEvent = false;
    if( _enabled ) _vrpnTracker->mainloop();        // TODO does this drain?
    return _updateReceivedEvent;
}

/*static*/ void Tracker::s_ChangeHandler( void* userdata, const vrpn_TRACKERCB info )
{
    static_cast<Tracker*>(userdata)->changeHandler( info );
}

void Tracker::changeHandler( const vrpn_TRACKERCB& info )
{
    _position[0] = _scale[0] * info.pos[0];
    _position[1] = _scale[1] * info.pos[1];
    _position[2] = _scale[2] * info.pos[2];
    _rotation.set( info.quat[0], info.quat[1], info.quat[2], info.quat[3] );

    _updateReceivedEvent = true;
}

// vim: set sw=4 ts=8 et ic ai:
