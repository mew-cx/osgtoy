/* file:        src/osgVRPN/TrackerManipulator.cpp
 * author:      Mike Weiblen
 * copyright:   (C) 2003-2006 Michael Weiblen http://mew.cx/
 * license:     OpenSceneGraph Public License (OSGPL)
 * $Id: TrackerManipulator.cpp,v 1.8 2006/07/15 23:54:58 mew Exp $
*/

#include <osgVRPN/TrackerManipulator.h>

using namespace osgVRPN;

///////////////////////////////////////////////////////////////////////////

TrackerManipulator::TrackerManipulator( osg::ref_ptr<TrackerBase> tracker )
{
    setTracker( tracker );
}

///////////////////////////////////////////////////////////////////////////

osg::Matrixd TrackerManipulator::getMatrix() const
{
    if( _tracker.valid() )
        return _tracker->getMatrix() * _homeMatrix;
    else
        return _homeMatrix;
}

osg::Matrixd TrackerManipulator::getInverseMatrix() const
{
    if( _tracker.valid() )
        return osg::Matrixd::inverse(_homeMatrix) * _tracker->getInverseMatrix();
    else
        return osg::Matrixd::inverse(_homeMatrix);
}

///////////////////////////////////////////////////////////////////////////

void TrackerManipulator::setNode(osg::Node* node)
{
    _node = node;
    if( _node.get() )
    {
        //const osg::BoundingSphere& boundingSphere=_node->getBound();
        //_modelScale = boundingSphere._radius;
    }
    if( getAutoComputeHomePosition() ) computeHomeMatrix();
}

///////////////////////////////////////////////////////////////////////////

void TrackerManipulator::computeHomeMatrix()
{
    if( _node.get() )
    {
        const osg::BoundingSphere& boundingSphere = _node->getBound();
        setHomeMatrix( osg::Matrix::translate( 0, 0, 2*boundingSphere._radius ) *
                osg::Matrixd::rotate( 0.9, 1, 0, 0 ) );

//osg::Matrix viewMatrix;
//viewMatrix.makeLookAt( bs.center()-osg::Vec3(0,2*bs.radius(),0), bs.center(), osg::Vec3(0,0,1) );
//setHomeMatrix( viewMatrix );

    }
}

void TrackerManipulator::setHomePosition(const osg::Vec3d& eye, const osg::Vec3d& center, const osg::Vec3d& up, bool autoComputeHomePosition)
{
    setAutoComputeHomePosition( autoComputeHomePosition );
// FUTURE : integrate with _homeMatrix
    _homeEye = eye;
    _homeCenter = center;
    _homeUp = up;
}
        
void TrackerManipulator::getHomePosition(osg::Vec3d& eye, osg::Vec3d& center, osg::Vec3d& up) const
{
// FUTURE : integrate with _homeMatrix
    eye = _homeEye;
    center = _homeCenter;
    up = _homeUp;
}

void TrackerManipulator::computeHomePosition()
{
// FUTURE : integrate with _homeMatrix
}

void TrackerManipulator::home( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us )
{
}

void TrackerManipulator::home( double currentTime )
{
}

///////////////////////////////////////////////////////////////////////////
// Handle "input events":
// VRPN doesn't generate OSG input events, so we use osgGA's FRAME event
// to ensure we get called often enough to process incoming VRPN messages.

bool TrackerManipulator::handle( const osgGA::GUIEventAdapter& ea,
        osgGA::GUIActionAdapter& )
{
    if( ea.getEventType() != osgGA::GUIEventAdapter::FRAME ) return false;
    if( _tracker.valid() ) _tracker->update();
    return true;
}

// vim: set sw=4 ts=8 et ic ai:
