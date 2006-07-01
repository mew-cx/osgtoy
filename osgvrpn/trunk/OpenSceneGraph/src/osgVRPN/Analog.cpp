/* file:        src/osgVRPN/Analog.cpp
 * author:      Mike Weiblen mew@mew.cx
 * copyright:   (C) 2003-2006 Michael Weiblen
 * license:     OpenSceneGraph Public License (OSGPL)
 * $Id: Analog.cpp,v 1.3 2006/07/01 20:48:52 mew Exp $
*/

#include <osg/Notify>
#include <osgVRPN/Analog.h>

#define VRPN_CLIENT_ONLY
#include "vrpn_Analog.h"

using namespace osgVRPN;

///////////////////////////////////////////////////////////////////////////

Analog::Analog( const char* deviceName ) :
        _vrpnAnalog( new vrpn_Analog_Remote(deviceName) ),
        _data( new osg::FloatArray ), _enabled(true)
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

bool Analog::update()
{
    _updateReceivedEvent = false;
    if( _enabled ) _vrpnAnalog->mainloop();     // TODO does this drain all msgs?
    return _updateReceivedEvent;
}

/*static*/ void Analog::s_ChangeHandler( void* userdata, const vrpn_ANALOGCB info )
{
    static_cast<Analog*>(userdata)->changeHandler( info );
}

void Analog::changeHandler( const vrpn_ANALOGCB& info )
{
    // TODO: ensure _data->capacity() is not reduced.
    _data->clear();
    // _data->erase( _data->begin(), _data->end() );

    _data->reserve( info.num_channel );
    for( int i = 0; i < info.num_channel; ++i ) _data->push_back( info.channel[i] );

    _updateReceivedEvent = true;
}

// vim: set sw=4 ts=8 et ic ai:
