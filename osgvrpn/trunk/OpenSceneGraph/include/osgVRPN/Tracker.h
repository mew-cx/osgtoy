/* file:        include/osgVRPN/Tracker.h
 * author:      Mike Weiblen mew@mew.cx
 * copyright:   (C) 2003-2006 Michael Weiblen
 * license:     OpenSceneGraph Public License (OSGPL)
 * $Id: Tracker.h,v 1.5 2006/07/01 20:48:51 mew Exp $
 *
 * references:  http://www.openscenegraph.org/
 *              http://www.vrpn.org/
 *              http://mew.cx/
*/

#ifndef OSGVRPN_TRACKER
#define OSGVRPN_TRACKER 1

#include <osg/Quat>
#include <osg/Vec3>
#include <osg/Matrixd>

#include <osgVRPN/Export>
#include <osgVRPN/TrackerBase.h>

// fwd declarations so OSG app code doesn't need the VRPN headers
class vrpn_Tracker_Remote;
typedef struct _vrpn_TRACKERCB vrpn_TRACKERCB;

namespace osgVRPN {

/** an OSG wrapper for VRPN's vrpn_Tracker_Remote class. */

class OSGVRPN_EXPORT Tracker : public TrackerBase
{
public:
    Tracker( const char* deviceName );

    /** Update our state from the device. */
    bool update();

    /** Query the tracker's transform matrix */
    osg::Matrixd getMatrix() const;
    osg::Matrixd getInverseMatrix() const;

protected:      // methods
    Tracker();
    Tracker(const Tracker&);
    virtual ~Tracker();
    const Tracker& operator=(const Tracker&);

    static void s_ChangeHandler( void* userdata, const vrpn_TRACKERCB info );
    void changeHandler( const vrpn_TRACKERCB& info );

protected:      // data
    vrpn_Tracker_Remote* const  _vrpnTracker;
    osg::Vec3                   _position;
    osg::Quat                   _rotation;
};

}

#endif

// vim: set sw=4 ts=8 et ic ai:
