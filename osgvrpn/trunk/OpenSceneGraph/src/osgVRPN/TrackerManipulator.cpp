/* file:	src/osgVRPN/TrackerManipulator.cpp
 * author:	Mike Weiblen mew@mew.cx 2003-11-30
 * copyright:	(C) 2003 Michael Weiblen
 * license:	OpenSceneGraph Public License (OSGPL)
*/

// #include <cassert>
#include <osg/Notify>
#include <osgVRPN/TrackerManipulator.h>

using namespace osgVRPN;

///////////////////////////////////////////////////////////////////////////

TrackerManipulator::TrackerManipulator( osg::ref_ptr<Tracker> tracker )
{
    setTracker( tracker );
}

TrackerManipulator::TrackerManipulator( const char* trackerName )
{
    setTracker( new Tracker( trackerName ) );
}

///////////////////////////////////////////////////////////////////////////

void TrackerManipulator::setByMatrix( const osg::Matrixd& matrix )
{
    // TODO how does OSG's use fit with this implementation?
    _origin = matrix;
}

void TrackerManipulator::setByInverseMatrix( const osg::Matrixd& matrix )
{
    // TODO how does OSG's use fit with this implementation?
    setByMatrix( osg::Matrixd::inverse(matrix) );
}

osg::Matrixd TrackerManipulator::getMatrix() const
{
    osg::Matrixd matrix;

    if( _tracker.valid() )
	matrix = _tracker->getMatrix() * _origin;
    else
	matrix = _origin;

    return matrix;
}

osg::Matrixd TrackerManipulator::getInverseMatrix() const
{
    osg::Matrixd matrix;

    if( _tracker.valid() )
	matrix = osg::Matrixd::inverse(_origin) * _tracker->getInverseMatrix();
    else
	matrix = osg::Matrixd::inverse(_origin);

    return matrix;
}

///////////////////////////////////////////////////////////////////////////

void TrackerManipulator::setNode(osg::Node* node)
{
    _node = node;
    if( _node.get() )
    {
        const osg::BoundingSphere& boundingSphere=_node->getBound();

	setByMatrix( osg::Matrix::translate( 0, 0, 2*boundingSphere._radius ) *
		osg::Matrixd::rotate( 0.9, 1, 0, 0 ) );
    }
}

///////////////////////////////////////////////////////////////////////////
// Handle "input events".
// VRPN traffic doesn't cause OSG input events, so we depend on the
// GUIEventAdapter::FRAME event to ensure we get called often enough to
// process VRPN messages.

bool TrackerManipulator::handle(
	const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& )
{
    switch( ea.getEventType() )
    {
	case osgGA::GUIEventAdapter::FRAME:
	{
	    if( _tracker.valid() )
		_tracker->update();
	    return true;
	}

	default:
	    break;
    }

    return false;
}

/*EOF*/
