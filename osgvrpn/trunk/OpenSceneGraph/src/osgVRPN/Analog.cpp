/* file:	src/osgVRPN/Analog.cpp
 * author:	Mike Weiblen mew@mew.cx 2003-12-27
 * copyright:	(C) 2003 Michael Weiblen
 * license:	OpenSceneGraph Public License (OSGPL)
*/

#include <cassert>
#include <osg/Notify>
#include <osgVRPN/Analog.h>

#define VRPN_CLIENT_ONLY
#include "vrpn_Analog.h"

using namespace osgVRPN;

///////////////////////////////////////////////////////////////////////////

Analog::Analog( const char* analogName )
{
    _scale = 1.0f;

    osg::notify(osg::INFO) << "Analog: attempting to open VRPN analog \""
	    << analogName << "\"." << std::endl;

    _vrpnAnalog = new vrpn_Analog_Remote( analogName );
    if( _vrpnAnalog )
    {
	_vrpnAnalog->register_change_handler( this, ChangeHandler );
    }
    else
    {
	osg::notify(osg::WARN) << "Analog: cannot open VRPN analog \""
		<< analogName << "\"." << std::endl;
    }
}

Analog::~Analog()
{
    delete _vrpnAnalog;
}

///////////////////////////////////////////////////////////////////////////

osg::Matrixd Analog::getMatrix() const
{
    return osg::Matrixd::rotate( _rotation ) *
	    osg::Matrixd::translate( _position );
}

osg::Matrixd Analog::getInverseMatrix() const
{
    return osg::Matrixd::translate( -_position ) *
	    osg::Matrixd::rotate( _rotation.inverse() );
}

///////////////////////////////////////////////////////////////////////////
// Interface to the VRPN message dispatch:
// update() is to be called "often" (e.g.: every frame) to receive messages
// from the VRPN server.
// For each message received, VRPN will invoke the ChangeHandler() callback.

void Analog::update()
{
    if( _vrpnAnalog )
    {
	_vrpnAnalog->mainloop();
    }
}

/*static*/ void Analog::ChangeHandler( void* userdata, const vrpn_ANALOGCB info )
{
    // userdata contains our "this" pointer
    static_cast<Analog*>( userdata )->changeHandler( &info );
}

void Analog::changeHandler( const vrpn_ANALOGCB* info )
{
    osg::notify(osg::INFO) << "Analog: "
            " num_channel=" << info->num_channel <<
            " C0=" << info->channel[0] <<
            " C1=" << info->channel[1] <<
            " C2=" << info->channel[2] <<
            " C3=" << info->channel[3] <<
            " C4=" << info->channel[4] <<
            " C5=" << info->channel[5] <<
            std::endl;

    _position.x() = _scale * info->channel[0];
    _position.y() = _scale * info->channel[1];
    _position.z() = _scale * info->channel[2];
#if 0 //[
    _rotation.set( info->quat[0], info->quat[1], info->quat[2], info->quat[3] );
#endif //]
}

/*EOF*/
