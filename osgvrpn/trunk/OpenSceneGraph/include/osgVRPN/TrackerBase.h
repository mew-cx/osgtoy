/* file:        include/osgVRPN/TrackerBase.h
 * author:      Mike Weiblen mew@mew.cx
 * copyright:   (C) 2006 Michael Weiblen
 * license:     OpenSceneGraph Public License (OSGPL)
 * $Id: TrackerBase.h,v 1.1 2006/06/23 17:22:37 mew Exp $
 *
 * references:  http://www.openscenegraph.org/
 *              http://www.vrpn.org/
 *              http://mew.cx/
*/

#ifndef OSGVRPN_TRACKERBASE
#define OSGVRPN_TRACKERBASE 1

#include <osg/Vec3>
#include <osg/Matrixd>
#include <osg/Referenced>

#include <osgVRPN/Export>

namespace osgVRPN {

/** a base class for osgVRPN trackers */

class OSGVRPN_EXPORT TrackerBase : public osg::Referenced
{
public:
    TrackerBase() : _scale(1.0f,1.0f,1.0f) {}

    /** Update our state from the device. */
    virtual void update() = 0;

    /** Query the tracker's transform matrix */
    virtual osg::Matrixd getMatrix() const = 0;
    virtual osg::Matrixd getInverseMatrix() const = 0;

    /** Set/query position scaling factor */
    void setScale( osg::Vec3& scale ) { _scale = scale; }
    osg::Vec3 getScale() const { return _scale; }

protected:      // methods
    TrackerBase(const TrackerBase&);
    virtual ~TrackerBase() {};
    const TrackerBase& operator=(const TrackerBase&);

protected:      // data
    osg::Vec3                   _scale;
};

}

#endif

// vim: set sw=4 ts=8 et ic ai:
