/* file:        include/osgVRPN/TrackerManipulator.h
 * author:      Mike Weiblen mew@mew.cx
 * copyright:   (C) 2003-2006 Michael Weiblen
 * license:     OpenSceneGraph Public License (OSGPL)
 * $Id: TrackerManipulator.h,v 1.4 2006/06/23 17:22:37 mew Exp $
 *
 * references:  http://www.openscenegraph.org/
 *              http://www.vrpn.org/
 *              http://mew.cx/
*/

#ifndef OSGVRPN_TRACKERMANIPULATOR
#define OSGVRPN_TRACKERMANIPULATOR 1

#include <osg/Matrixd>
#include <osgGA/MatrixManipulator>
#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIActionAdapter>

#include <osgVRPN/Export>
#include <osgVRPN/TrackerBase.h>

namespace osgVRPN {

/** osgVRPN::TrackerManipulator is a MatrixManipulator that can be controlled by an osgVRPN::TrackerBase.
It may be attached to an osgProducer::Viewer as a CameraManipulator
to control the position of the camera.
*/

class OSGVRPN_EXPORT TrackerManipulator : public osgGA::MatrixManipulator
{
public:
    TrackerManipulator( osg::ref_ptr<TrackerBase> tracker = 0 );
    TrackerManipulator( const char* trackerName );

    virtual const char* className() const { return "osgVRPN::TrackerManipulator"; }


    /** Set the position of this manipulator
    (stubbed-out, as they is not meaningful for this device) */
    virtual void setByMatrix(const osg::Matrixd& matrix) {}
    virtual void setByInverseMatrix(const osg::Matrixd& matrix) {}

    /** Query the position of this manipulator. */
    virtual osg::Matrixd getMatrix() const;
    virtual osg::Matrixd getInverseMatrix() const;


    /** Set the Node that determines this manipulator's origin */
    virtual void setNode(osg::Node*);

    /** Query the Node that determines this manipulator's origin */
    virtual osg::Node* getNode() { return _node.get(); }
    virtual const osg::Node* getNode() const { return _node.get(); }


    /** Set/get the home matrix. */
    void setHomeMatrix(const osg::Matrixd& matrix) {_homeMatrix = matrix;}
    osg::Matrixd getHomeMatrix() const {return _homeMatrix;}

    /** Compute a reasonable default home matrix. */
    virtual void computeHomeMatrix();

    // FUTURE
    void setHomePosition(const osg::Vec3d& eye, const osg::Vec3d& center, const osg::Vec3d& up, bool autoComputeHomePosition=false);
    void getHomePosition(osg::Vec3d& eye, osg::Vec3d& center, osg::Vec3d& up) const;
    void computeHomePosition();

    /** Move the camera to the default matrix. */
    virtual void home( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us );
    virtual void home( double currentTime );


    /** Set the tracker that controls this manipulator, 0==none */
    void setTracker( osg::ref_ptr<TrackerBase> tracker ) { _tracker = tracker; }

    /** Query the tracker that controls this manipulator. */
    osg::ref_ptr<TrackerBase> getTracker() const { return _tracker; }


    /** Process events, return true if handled, false otherwise. */
    virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us );


protected:      // methods
    virtual ~TrackerManipulator() {}

protected:      // data
    osg::ref_ptr<TrackerBase> _tracker;
    osg::ref_ptr<osg::Node> _node;
    osg::Matrixd _homeMatrix;
};

}

#endif

// vim: set sw=4 ts=8 et ic ai:
