/* file:        src/osgVRPN/AnalogTracker.cpp
 * author:      Mike Weiblen
 * copyright:   (C) 2006 Michael Weiblen http://mew.cx/
 * license:     OpenSceneGraph Public License (OSGPL)
 * $Id 2006-07-08 $
*/

#include <osgVRPN/AnalogTracker.h>
#include <osg/Quat>

using namespace osgVRPN;

///////////////////////////////////////////////////////////////////////////

AnalogTracker::AnalogTracker() :
        _analogDevice(0),
        _chTX(-1), _chTY(-1), _chTZ(-1),
        _chRX(-1), _chRY(-1), _chRZ(-1),
        _rotScale(1,1,1),
        _buttonDevice(0),
        _resetButton(-1), _previousResetState(false)
{
    reset();
}

///////////////////////////////////////////////////////////////////////////

void AnalogTracker::update()
{
    if( _analogDevice.valid() ) _analogDevice->clearEventCounter();
    if( _buttonDevice.valid() ) _buttonDevice->clearEventCounter();

    if( _analogDevice.valid() ) _analogDevice->update();
    if( _buttonDevice.valid() ) _buttonDevice->update();

    if( _analogDevice.valid() && _analogDevice->getEventCounter() > 0 )
    {
        Analog& ana = *(_analogDevice.get());

        osg::Vec3 pos;
        if( _chTX >= 0 ) pos[0] = _transScale[0] * ana.getValue(_chTX);
        if( _chTY >= 0 ) pos[1] = _transScale[1] * ana.getValue(_chTY);
        if( _chTZ >= 0 ) pos[2] = _transScale[2] * ana.getValue(_chTZ);

        osg::Quat qx, qy, qz;
        if( _chRX >= 0 ) qx.makeRotate( _rotScale[0] * ana.getValue(_chRX), osg::Vec3(1,0,0) );
        if( _chRY >= 0 ) qy.makeRotate( _rotScale[1] * ana.getValue(_chRY), osg::Vec3(0,1,0) );
        if( _chRZ >= 0 ) qz.makeRotate( _rotScale[2] * ana.getValue(_chRZ), osg::Vec3(0,0,1) );

        _matrix = osg::Matrixd::translate(pos) * osg::Matrixd::rotate(qx*qy*qz) * _matrix;

        _eventCounter += ana.getEventCounter();
    }

    if( _buttonDevice.valid() && _buttonDevice->getEventCounter() > 0 )
    {
        bool currentResetState = _buttonDevice->getButtonState( _resetButton );
        if( currentResetState && !_previousResetState ) reset();
        _previousResetState = currentResetState;

        _eventCounter += _buttonDevice->getEventCounter();
    }
}

// vim: set sw=4 ts=8 et ic ai:
