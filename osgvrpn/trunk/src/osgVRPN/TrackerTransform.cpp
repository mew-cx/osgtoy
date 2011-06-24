/* file:        src/osgVRPN/TrackerTransform.cpp
 * author:      Mike Weiblen
 * copyright:   (C) 2003-2006 Michael Weiblen http://mew.cx/
 * license:     OpenSceneGraph Public License (OSGPL)
 * $Id$
*/

#include <osgVRPN/TrackerTransform.h>

using namespace osgVRPN;

///////////////////////////////////////////////////////////////////////////

namespace {

class TrackerUpdateCallback : public osg::NodeCallback
{
    virtual void operator()( osg::Node* node, osg::NodeVisitor* nv )
    {
        TrackerTransform* xform( dynamic_cast<TrackerTransform*>(node) );
        TrackerBase* tracker( xform->getTracker().get() );
        if( tracker )
        {
            tracker->update();
            xform->setMatrix( tracker->getMatrix() );
        }

        traverse(node,nv);
    }
};

}

///////////////////////////////////////////////////////////////////////////

TrackerTransform::TrackerTransform( osg::ref_ptr<TrackerBase> tracker )
{
    setTracker( tracker );
    setUpdateCallback( new TrackerUpdateCallback );
}

// vim: set sw=4 ts=8 et ic ai:
