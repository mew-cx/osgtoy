/* file:        src/osgVRPN/Button.cpp
 * author:      Mike Weiblen
 * copyright:   (C) 2006 Michael Weiblen http://mew.cx/
 * license:     OpenSceneGraph Public License (OSGPL)
 * $Id 2006-07-08$
*/

#include <osgVRPN/Button.h>

#define VRPN_CLIENT_ONLY
#include "vrpn_Button.h"

using namespace osgVRPN;

///////////////////////////////////////////////////////////////////////////

Button::Button( const char* deviceName ) :
        _enabled(true), _eventCounter(0),
        _vrpnButton( new vrpn_Button_Remote(deviceName) )
{
    _vrpnButton->register_change_handler( this, s_ChangeHandler );
}

Button::~Button()
{
    _vrpnButton->unregister_change_handler( this, s_ChangeHandler );
    delete _vrpnButton;
}

///////////////////////////////////////////////////////////////////////////

bool Button::getButtonState( int button ) const
{
    ButtonStateMap::const_iterator itr = _buttonStates.find( button );
    return (itr!=_buttonStates.end()) ? itr->second : false;
}

///////////////////////////////////////////////////////////////////////////
// Interface to the VRPN message dispatch:
// update() is to be called "often" (e.g.: every frame) to receive messages
// from the VRPN server.
// For each message received, VRPN will invoke the s_ChangeHandler() callback.

void Button::update()
{
    if( _enabled ) _vrpnButton->mainloop();
}

/*static*/ void Button::s_ChangeHandler( void* userdata, const vrpn_BUTTONCB info )
{
    static_cast<Button*>(userdata)->changeHandler( info );
}

void Button::changeHandler( const vrpn_BUTTONCB& info )
{
    int button = info.button;
    bool state = (info.state == VRPN_BUTTON_ON);
    _buttonStates[button] = state;
    ++_eventCounter;
}

// vim: set sw=4 ts=8 et ic ai:
