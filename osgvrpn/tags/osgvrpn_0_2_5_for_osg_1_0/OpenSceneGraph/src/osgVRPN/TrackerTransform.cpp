/* file:        src/osgVRPN/TrackerTransform.cpp
 * author:      Mike Weiblen mew@mew.cx
 * copyright:   (C) 2003-2005 Michael Weiblen
 * license:     OpenSceneGraph Public License (OSGPL)
 * $Id: TrackerTransform.cpp,v 1.3 2005/11/09 08:29:00 mew Exp $
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
        osg::ref_ptr<Tracker> tracker = xform->getTracker();
        if( tracker.valid() )
        {
            tracker->update();
            xform->setMatrix( tracker->getMatrix() );
        }

        traverse(node,nv);
    }
};

///////////////////////////////////////////////////////////////////////////

TrackerTransform::TrackerTransform( osg::ref_ptr<Tracker> tracker )
{
    setTracker( tracker );
    setUpdateCallback( new TrackerUpdateCallback );
}

TrackerTransform::TrackerTransform( const char* trackerName )
{
    setTracker( new Tracker( trackerName ) );
    setUpdateCallback( new TrackerUpdateCallback );
}

// vim: set sw=4 ts=8 et ic ai:
