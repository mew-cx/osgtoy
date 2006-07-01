/* file:        src/osgVRPN/AnalogTracker.cpp
 * author:      Mike Weiblen mew@mew.cx
 * copyright:   (C) 2006 Michael Weiblen
 * license:     OpenSceneGraph Public License (OSGPL)
 * $Id: AnalogTracker.cpp,v 1.2 2006/07/01 20:48:52 mew Exp $
*/

#include <osg/Notify>
#include <osgVRPN/AnalogTracker.h>

using namespace osgVRPN;

///////////////////////////////////////////////////////////////////////////

AnalogTracker::AnalogTracker()
{
}

AnalogTracker::~AnalogTracker()
{
}

///////////////////////////////////////////////////////////////////////////

bool AnalogTracker::update()
{
    _updateReceivedEvent = false;
    // update the attached osgVRPN::Analog devices
#if 0 //[
    _position[0] = _scale[0] * info.pos[0];
    _position[1] = _scale[1] * info.pos[1];
    _position[2] = _scale[2] * info.pos[2];
    _rotation.set( info.quat[0], info.quat[1], info.quat[2], info.quat[3] );
    if( _enabled ) ...
#endif //]
    return _updateReceivedEvent;
}

///////////////////////////////////////////////////////////////////////////

osg::Matrixd AnalogTracker::getMatrix() const
{
#if 0 //[
    return osg::Matrixd::rotate( _rotation ) *
            osg::Matrixd::translate( _position );
#endif //]
    osg::Matrixd _m;
    return _m;
}

osg::Matrixd AnalogTracker::getInverseMatrix() const
{
#if 0 //[
    return osg::Matrixd::translate( -_position ) *
            osg::Matrixd::rotate( _rotation.inverse() );
#endif //]
    osg::Matrixd _m;
    return _m;
}

// vim: set sw=4 ts=8 et ic ai:
