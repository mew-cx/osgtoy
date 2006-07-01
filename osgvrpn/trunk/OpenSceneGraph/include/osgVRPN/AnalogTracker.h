/* file:        include/osgVRPN/AnalogTracker.h
 * author:      Mike Weiblen mew@mew.cx
 * copyright:   (C) 2006 Michael Weiblen
 * license:     OpenSceneGraph Public License (OSGPL)
 * $Id: AnalogTracker.h,v 1.2 2006/07/01 20:48:51 mew Exp $
 *
 * references:  http://www.openscenegraph.org/
 *              http://www.vrpn.org/
 *              http://mew.cx/
*/

#ifndef OSGVRPN_ANALOGTRACKER
#define OSGVRPN_ANALOGTRACKER 1

#include <osgVRPN/Export>
#include <osgVRPN/TrackerBase.h>

namespace osgVRPN {

/** an aggregate of vrpn_Analog_Remote channels. */

class OSGVRPN_EXPORT AnalogTracker : public TrackerBase
{
public:
    AnalogTracker();

    /** Update our state from the device. */
    bool update();

    /** Query the tracker's transform matrix */
    osg::Matrixd getMatrix() const;
    osg::Matrixd getInverseMatrix() const;

protected:      // methods
    AnalogTracker(const AnalogTracker&);
    virtual ~AnalogTracker();
    const AnalogTracker& operator=(const AnalogTracker&);

protected:      // data
};

}

#endif

// vim: set sw=4 ts=8 et ic ai:




