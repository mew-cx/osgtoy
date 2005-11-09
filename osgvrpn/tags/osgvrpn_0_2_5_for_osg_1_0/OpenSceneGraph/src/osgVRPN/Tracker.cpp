/* file:        src/osgVRPN/Tracker.cpp
 * author:      Mike Weiblen mew@mew.cx
 * copyright:   (C) 2003-2005 Michael Weiblen
 * license:     OpenSceneGraph Public License (OSGPL)
 * $Id: Tracker.cpp,v 1.3 2005/11/09 08:29:00 mew Exp $
*/

#include <osg/Notify>
#include <osgVRPN/Tracker.h>

#define VRPN_CLIENT_ONLY
#include "vrpn_Tracker.h"

using namespace osgVRPN;

///////////////////////////////////////////////////////////////////////////

Tracker::Tracker( const char* trackerName ) : _scale(1.0f)
{
    osg::notify(osg::INFO) << "Tracker: attempting to open VRPN tracker \""
            << trackerName << "\"." << std::endl;

    _vrpnTracker = new vrpn_Tracker_Remote( trackerName );
    _vrpnTracker->register_change_handler( this, ChangeHandler );
}

Tracker::~Tracker()
{
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
// For each message received, VRPN will invoke the ChangeHandler() callback.

void Tracker::update()
{
    if( _vrpnTracker ) _vrpnTracker->mainloop();
}

/*static*/ void Tracker::ChangeHandler( void* userdata, const vrpn_TRACKERCB info )
{
    // userdata contains our "this" pointer
    static_cast<Tracker*>( userdata )->changeHandler( &info );
}

void Tracker::changeHandler( const vrpn_TRACKERCB* info )
{
    _position.x() = _scale * info->pos[0];
    _position.y() = _scale * info->pos[1];
    _position.z() = _scale * info->pos[2];
    _rotation.set( info->quat[0], info->quat[1], info->quat[2], info->quat[3] );
}

// vim: set sw=4 ts=8 et ic ai:
