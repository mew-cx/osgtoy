/* file:        include/osgVRPN/TrackerTransform.h
 * author:      Mike Weiblen mew@mew.cx
 * copyright:   (C) 2003-2006 Michael Weiblen
 * license:     OpenSceneGraph Public License (OSGPL)
 * $Id: TrackerTransform.h,v 1.4 2006/06/23 17:22:37 mew Exp $
 *
 * references:  http://www.openscenegraph.org/
 *              http://www.vrpn.org/
 *              http://mew.cx/
*/

#ifndef OSG_TRACKERTRANSFORM
#define OSG_TRACKERTRANSFORM 1

#include <osg/MatrixTransform>

#include <osgVRPN/Export>
#include <osgVRPN/TrackerBase.h>

namespace osgVRPN {

/** osgVRPN::TrackerTransform is a Transform node that can be controlled by a osgVRPN::TrackerBase.
It can be used to control the position of objects in the scene.
*/

class OSGVRPN_EXPORT TrackerTransform : public osg::MatrixTransform
{
public :
    TrackerTransform( osg::ref_ptr<TrackerBase> tracker = 0 );

    TrackerTransform( const TrackerTransform& xform,
            const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY ) :
                MatrixTransform( xform, copyop ),
                _tracker( xform._tracker )
    {}

    META_Node( osgVRPN, TrackerTransform );


    /** Set the tracker that controls this transform, 0==none */
    void setTracker( osg::ref_ptr<TrackerBase> tracker ) { _tracker = tracker; }

    /** Query the tracker that controls this transform. */
    osg::ref_ptr<TrackerBase> getTracker() const { return _tracker; }

protected:      // methods
    virtual ~TrackerTransform() {}

protected:      // data
    osg::ref_ptr<TrackerBase> _tracker;
};

}

#endif

// vim: set sw=4 ts=8 et ic ai:
