/* file:        src/osgVRPN/TrackerTransform.cpp
 * author:      Mike Weiblen mew@mew.cx
 * copyright:   (C) 2003-2006 Michael Weiblen
 * license:     OpenSceneGraph Public License (OSGPL)
 * $Id: TrackerTransform.cpp,v 1.4 2006/06/23 17:22:37 mew Exp $
*/

#include <osgVRPN/TrackerTransform.h>

using namespace osgVRPN;

// TODO add a feature to rotate the Tracker into the view orientation.

///////////////////////////////////////////////////////////////////////////

class TrackerUpdateCallback : public osg::NodeCallback
{
    virtual void operator()( osg::Node* node, osg::NodeVisitor* nv )
    {
        TrackerTransform* xform = dynamic_cast<TrackerTransform*>( node );
        osg::ref_ptr<TrackerBase> tracker = xform->getTracker();
        if( tracker.valid() )
        {
            tracker->update();
            xform->setMatrix( tracker->getMatrix() );
        }

        traverse(node,nv);
    }
};

///////////////////////////////////////////////////////////////////////////

TrackerTransform::TrackerTransform( osg::ref_ptr<TrackerBase> tracker )
{
    setTracker( tracker );
    setUpdateCallback( new TrackerUpdateCallback );
}

// vim: set sw=4 ts=8 et ic ai:
