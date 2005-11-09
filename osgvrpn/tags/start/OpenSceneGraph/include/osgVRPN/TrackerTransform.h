/* file:	include/osgVRPN/TrackerTransform.h
 * author:	Mike Weiblen mew@mew.cx 2003-12-28
 * copyright:	(C) 2003 Michael Weiblen
 * license:	OpenSceneGraph Public License (OSGPL)
 *
 * references:	http://www.openscenegraph.org/
 *		http://www.vrpn.org/
 *		http://www.mew.cx/osg/
 *
 * depends:	OSG 0.9.6-2, VRPN 06.04
*/

#ifndef OSG_TRACKERTRANSFORM
#define OSG_TRACKERTRANSFORM 1

#include <osg/MatrixTransform>

#include <osgVRPN/Export>
#include <osgVRPN/Tracker.h>

namespace osgVRPN {

/** osgVRPN::TrackerTransform is a Transform node that can be controlled by a osgVRPN::Tracker.
It can be used to control the position of objects in the scene.
*/

class OSGVRPN_EXPORT TrackerTransform : public osg::MatrixTransform
{
public :
    TrackerTransform( osg::ref_ptr<Tracker> tracker = 0 );
    TrackerTransform( const char* trackerName );

    TrackerTransform( const TrackerTransform& xform,
	    const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY ) :
		MatrixTransform( xform, copyop ),
		_tracker( xform._tracker )
    {}

    META_Node( osgVRPN, TrackerTransform );


    /** Set the Tracker that controls this transform, 0==none */
    void setTracker( osg::ref_ptr<Tracker> tracker ) { _tracker = tracker; }

    /** Query the Tracker that controls this transform. */
    osg::ref_ptr<Tracker> getTracker() const { return _tracker; }


protected:	// methods
    virtual ~TrackerTransform() {}

protected:	// data
    osg::ref_ptr<Tracker> _tracker;
};

}

#endif
