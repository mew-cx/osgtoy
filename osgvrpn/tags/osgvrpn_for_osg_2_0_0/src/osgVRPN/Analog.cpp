/* file:        src/osgVRPN/Analog.cpp
 * author:      Mike Weiblen
 * copyright:   (C) 2003-2006 Michael Weiblen http://mew.cx/
 * license:     OpenSceneGraph Public License (OSGPL)
 * $Id: Analog.cpp,v 1.6 2006/07/15 23:54:58 mew Exp $
*/

#include <osgVRPN/Analog.h>

#define VRPN_CLIENT_ONLY
#include "vrpn_Analog.h"

using namespace osgVRPN;

///////////////////////////////////////////////////////////////////////////

Analog::Analog( const char* deviceName ) :
        _enabled(true), _eventCounter(0),
        _vrpnAnalog( new vrpn_Analog_Remote(deviceName) ),
        _data( new osg::FloatArray )
{
    _vrpnAnalog->register_change_handler( this, s_ChangeHandler );
}

Analog::~Analog()
{
    _vrpnAnalog->unregister_change_handler( this, s_ChangeHandler );
    delete _vrpnAnalog;
}

///////////////////////////////////////////////////////////////////////////
// Interface to the VRPN message dispatch:
// update() is to be called "often" (e.g.: every frame) to receive messages
// from the VRPN server.
// For each message received, VRPN will invoke the s_ChangeHandler() callback.

void Analog::update()
{
    if( _enabled ) _vrpnAnalog->mainloop();
}

/*static*/ void Analog::s_ChangeHandler( void* userdata, const vrpn_ANALOGCB info )
{
    static_cast<Analog*>(userdata)->changeHandler( info );
}

void Analog::changeHandler( const vrpn_ANALOGCB& info )
{
    _data->resize( info.num_channel );
    for( int i = 0; i < info.num_channel; ++i ) (*_data)[i] = info.channel[i];
    ++_eventCounter;
}

// vim: set sw=4 ts=8 et ic ai:
