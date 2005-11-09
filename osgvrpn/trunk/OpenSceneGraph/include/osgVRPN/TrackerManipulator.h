/* file:	include/osgVRPN/TrackerManipulator.h
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

#ifndef OSGVRPN_TRACKERMANIPULATOR
#define OSGVRPN_TRACKERMANIPULATOR 1

#include <osg/Matrixd>
#include <osgGA/MatrixManipulator>
#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIActionAdapter>

#include <osgVRPN/Export>
#include <osgVRPN/Tracker.h>

namespace osgVRPN {

/** osgVRPN::TrackerManipulator is a MatrixManipulator that can be controlled by an osgVRPN::Tracker.
It may be attached to an osgProducer::Viewer as a CameraManipulator
to control the position of the camera.
*/

class OSGVRPN_EXPORT TrackerManipulator : public osgGA::MatrixManipulator
{
public:
    TrackerManipulator( osg::ref_ptr<Tracker> tracker = 0 );
    TrackerManipulator( const char* trackerName );

    virtual const char* className() const { return "osgVRPN::TrackerManipulator"; }


    /** Set the position of this manipulator */
    // TODO what should this mean for a read-only device like a Tracker??
    virtual void setByMatrix(const osg::Matrixd& matrix);
    virtual void setByInverseMatrix(const osg::Matrixd& matrix);

    /** Query the position of this manipulator. */
    virtual osg::Matrixd getMatrix() const;
    virtual osg::Matrixd getInverseMatrix() const;


    /** Set the Node that determines this manipulator's origin */
    virtual void setNode(osg::Node*);

    /** Query the Node that determines this manipulator's origin */
    virtual osg::Node* getNode() { return _node.get(); }
    virtual const osg::Node* getNode() const { return _node.get(); }


    /** Set the Tracker that controls this manipulator, 0==none */
    void setTracker( osg::ref_ptr<Tracker> tracker ) { _tracker = tracker; }

    /** Query the Tracker that controls this manipulator. */
    osg::ref_ptr<Tracker> getTracker() const { return _tracker; }


    /** Process events, return true if handled, false otherwise. */
    virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us );


protected:	// methods
    virtual ~TrackerManipulator() {}

protected:	// data
    osg::ref_ptr<Tracker> _tracker;
    osg::ref_ptr<osg::Node> _node;
    osg::Matrixd _origin;
};

}

#endif
