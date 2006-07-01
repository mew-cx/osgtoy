/* file:        src/osgVRPN/TrackerTransform.cpp
 * author:      Mike Weiblen mew@mew.cx
 * copyright:   (C) 2003-2006 Michael Weiblen
 * license:     OpenSceneGraph Public License (OSGPL)
 * $Id: TrackerTransform.cpp,v 1.5 2006/07/01 20:48:52 mew Exp $
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
            for( bool more=true; more; more=tracker->update() ) {}
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
