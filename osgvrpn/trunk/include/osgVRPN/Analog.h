/* file:        include/osgVRPN/Analog.h
 * author:      Mike Weiblen
 * copyright:   (C) 2003-2007 Michael Weiblen http://mew.cx/
 * license:     OpenSceneGraph Public License (OSGPL)
 * $Id$
*/

#ifndef OSGVRPN_ANALOG
#define OSGVRPN_ANALOG 1

#include <osg/Referenced>
#include <osg/ref_ptr>
#include <osg/Array>

#include <osgVRPN/Export.h>

#include <vrpn_Configure.h>
// fwd declarations to reduce need for VRPN headers
class vrpn_Analog_Remote;
typedef struct _vrpn_ANALOGCB vrpn_ANALOGCB;

namespace osgVRPN {

/** an OSG wrapper for VRPN's vrpn_Analog_Remote class. */

class OSGVRPN_EXPORT Analog : public osg::Referenced
{
public:
    Analog( const char* deviceName );

    /** Set/get update enable flag */
    void setEnable( bool enabled ) { _enabled = enabled; }
    bool getEnable() const { return _enabled; }

    /** Update our state from the device. */
    void update();

    void clearEventCounter() {_eventCounter = 0;}
    unsigned int getEventCounter() const {return _eventCounter;}

    /** Query a channel value from the Analog */
    unsigned int getNumChannels() const { return _data->getNumElements(); }
    float getValue(unsigned int channel) const { return _data->at(channel); }

protected:      // methods
    ~Analog();

    static void VRPN_CALLBACK s_ChangeHandler( void* userdata, const vrpn_ANALOGCB info );
    void changeHandler( const vrpn_ANALOGCB& info );

protected:      // data
    bool _enabled;
    unsigned int _eventCounter;
    vrpn_Analog_Remote* const _vrpnAnalog;
    osg::ref_ptr<osg::FloatArray> _data;

private:        // uncopyable
    Analog(const Analog&);
    Analog& operator=(const Analog&);
};

}

#endif

// vim: set sw=4 ts=8 et ic ai:
