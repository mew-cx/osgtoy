/* file:        include/osgVRPN/Button.h
 * author:      Mike Weiblen
 * copyright:   (C) 2006-2011 Michael Weiblen http://mew.cx/
 * license:     OpenSceneGraph Public License (OSGPL)
 * $Id$
*/

#ifndef OSGVRPN_BUTTON
#define OSGVRPN_BUTTON 1

#include <map>
#include <osg/Referenced>

#include <osgVRPN/Export.h>

#include <vrpn_Configure.h>
// fwd declarations to reduce need for VRPN headers
class vrpn_Button_Remote;
typedef struct _vrpn_BUTTONCB vrpn_BUTTONCB;

namespace osgVRPN {

/** an OSG wrapper for VRPN's vrpn_Button_Remote class. */

class OSGVRPN_EXPORT Button : public osg::Referenced
{
public:
    Button( const char* deviceName );

    /** Set/get update enable flag */
    void setEnable( bool enabled ) { _enabled = enabled; }
    bool getEnable() const { return _enabled; }

    /** Update our state from the device. */
    void update();

    void clearEventCounter() {_eventCounter = 0;}
    unsigned int getEventCounter() const {return _eventCounter;}

    /** Get state of a button */
    bool getButtonState( int buttonId ) const;

protected:      // methods
    ~Button();

    static void VRPN_CALLBACK s_ChangeHandler( void* userdata, const vrpn_BUTTONCB info );
    void changeHandler( const vrpn_BUTTONCB& info );

protected:      // data
    bool _enabled;
    unsigned int _eventCounter;
    vrpn_Button_Remote* const _vrpnButton;

    typedef std::map<int,bool> ButtonStateMap;
    ButtonStateMap _buttonStates;

private:        // uncopyable
    Button(const Button&);
    Button& operator=(const Button&);
};

}

#endif

// vim: set sw=4 ts=8 et ic ai:
