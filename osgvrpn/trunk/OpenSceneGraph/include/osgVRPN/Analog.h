/* file:	include/osgVRPN/Analog.h
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

#ifndef OSGVRPN_ANALOG
#define OSGVRPN_ANALOG 1

#include <osg/Quat>
#include <osg/Vec3>
#include <osg/Matrixd>
#include <osg/Referenced>

#include <osgVRPN/Export>

// fwd declarations so OSG app code doesn't need the VRPN headers
class vrpn_Analog_Remote;
typedef struct _vrpn_ANALOGCB vrpn_ANALOGCB;

namespace osgVRPN {

/** osgVRPN::Analog is an OSG wrapper for VRPN's vrpn_Analog_Remote class.  */

class OSGVRPN_EXPORT Analog : public osg::Referenced
{
public:
    Analog( const char* analogName );

    /** Obtain our associated matrices */
    virtual osg::Matrixd getMatrix() const;
    virtual osg::Matrixd getInverseMatrix() const;

    /** Update our state by draining pending messages from the VRPN server */
    void update();

    /** Set our position scaling factor */
    void setScale( float scale ) { _scale = scale; }

    /** Query our position scaling factor */
    float getScale() const { return _scale; }


protected:	// methods
    Analog();
    virtual ~Analog();
    Analog(const Analog&);
    const Analog& operator=(const Analog&);

    static void ChangeHandler( void* userdata, const vrpn_ANALOGCB info );
    void changeHandler( const vrpn_ANALOGCB* info );

protected:	// data
    float			_scale;
    osg::Vec3			_position;
    osg::Quat			_rotation;
    vrpn_Analog_Remote*	_vrpnAnalog;
};

}

#endif
