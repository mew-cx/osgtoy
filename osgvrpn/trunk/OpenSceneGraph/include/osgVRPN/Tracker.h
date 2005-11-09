/* file:	include/osgVRPN/Tracker.h
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

#ifndef OSGVRPN_TRACKER
#define OSGVRPN_TRACKER 1

#include <osg/Quat>
#include <osg/Vec3>
#include <osg/Matrixd>
#include <osg/Referenced>

#include <osgVRPN/Export>

// fwd declarations so OSG app code doesn't need the VRPN headers
class vrpn_Tracker_Remote;
typedef struct _vrpn_TRACKERCB vrpn_TRACKERCB;

namespace osgVRPN {

/** osgVRPN::Tracker is an OSG wrapper for VRPN's vrpn_Tracker_Remote class.  */

class OSGVRPN_EXPORT Tracker : public osg::Referenced
{
public:
    Tracker( const char* trackerName );

    /** Query the position of the Tracker */
    virtual osg::Matrixd getMatrix() const;
    virtual osg::Matrixd getInverseMatrix() const;

    /** Update our state by draining pending messages from the VRPN server */
    void update();

    /** Set our position scaling factor */
    void setScale( float scale ) { _scale = scale; }

    /** Query our position scaling factor */
    float getScale() const { return _scale; }


protected:	// methods
    Tracker();
    Tracker(const Tracker&);
    virtual ~Tracker();
    const Tracker& operator=(const Tracker&);

    static void ChangeHandler( void* userdata, const vrpn_TRACKERCB info );
    void changeHandler( const vrpn_TRACKERCB* info );

protected:	// data
    float			_scale;
    osg::Vec3			_position;
    osg::Quat			_rotation;
    vrpn_Tracker_Remote*	_vrpnTracker;
};

}

#endif
