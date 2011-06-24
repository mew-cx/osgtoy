/* file:        include/osgVRPN/TrackerManipulator.h
 * author:      Mike Weiblen
 * copyright:   (C) 2003-2011 Michael Weiblen http://mew.cx/
 * license:     OpenSceneGraph Public License (OSGPL)
 * $Id$
*/

#ifndef OSGVRPN_TRACKERMANIPULATOR
#define OSGVRPN_TRACKERMANIPULATOR 1

#include <osg/ref_ptr>
#include <osg/Matrixd>
#include <osgGA/MatrixManipulator>
#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIActionAdapter>
#include <osgVRPN/TrackerBase.h>

#include <osgVRPN/Export.h>

namespace osgVRPN {

/** a MatrixManipulator controlled by an osgVRPN::TrackerBase.
It may be attached to an osgViewer::Viewer as a CameraManipulator
to control the position of the camera.
*/

class OSGVRPN_EXPORT TrackerManipulator : public osgGA::MatrixManipulator
{
public:
    TrackerManipulator( osg::ref_ptr<TrackerBase> tracker = 0 );

    const char* className() const { return "osgVRPN::TrackerManipulator"; }

    /** Set the position of this manipulator
    (not meaningful, this manipulator is read-only) */
    void setByMatrix(const osg::Matrixd& matrix) {}
    void setByInverseMatrix(const osg::Matrixd& matrix) {}

    /** Query the position of this manipulator. */
    osg::Matrixd getMatrix() const;
    osg::Matrixd getInverseMatrix() const;

    /** Set/get the Node that determines this manipulator's origin */
    void setNode(osg::Node*);
    osg::Node* getNode() { return _node.get(); }
    const osg::Node* getNode() const { return _node.get(); }

    /** Set/get the home matrix. */
    void setHomeMatrix(const osg::Matrixd& matrix) {_homeMatrix = matrix;}
    osg::Matrixd getHomeMatrix() const {return _homeMatrix;}

    /** Compute a reasonable default home matrix. */
    void computeHomeMatrix();

    // FUTURE
    void setHomePosition(const osg::Vec3d& eye, const osg::Vec3d& center, const osg::Vec3d& up, bool autoComputeHomePosition=false);
    void getHomePosition(osg::Vec3d& eye, osg::Vec3d& center, osg::Vec3d& up) const;
    void computeHomePosition();

    /** Move the camera to the default matrix. */
    void home( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us );
    void home( double currentTime );

    /** Set/get the tracker that controls this manipulator, 0==none */
    void setTracker( osg::ref_ptr<TrackerBase> tracker ) { _tracker = tracker; }
    osg::ref_ptr<TrackerBase> getTracker() const { return _tracker; }

    /** Process events, return true if handled, false otherwise. */
    bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us );

protected:      // methods
    ~TrackerManipulator() {}

protected:      // data
    osg::ref_ptr<TrackerBase> _tracker;
    osg::ref_ptr<osg::Node> _node;
    osg::Matrixd _homeMatrix;
};

}

#endif

// vim: set sw=4 ts=8 et ic ai:
