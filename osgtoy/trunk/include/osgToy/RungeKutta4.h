/* file:        include/osgToy/RungeKutta4
 * author:      Mike Weiblen 2004-12-22
 * copyright:   (C) 2004 Michael Weiblen
 * license:     OpenSceneGraph Public License (OSGPL)
 * website:     http://mew.cx/osg/
 * $Id: RungeKutta4,v 1.2 2006/09/11 06:24:04 mew Exp $
*/

#ifndef OSGTOY_RUNGEKUTTA4
#define OSGTOY_RUNGEKUTTA4 1

#include <osg/Vec3>

#include <osgToy/Export.h>

namespace osgToy {

/** 4th order Runge-Kutta ODE solver */

class OSGTOY_EXPORT RungeKutta4
{
public:
    RungeKutta4( osg::Vec3 initialP, float initialT ) :
            _p( initialP ), _t( initialT ) {}
    virtual ~RungeKutta4() {}

    const osg::Vec3& p() const { return _p; }
    float t() const { return _t; }

    void advance( float h );

protected:
    virtual osg::Vec3 dpdt( const osg::Vec3&, float ) = 0;

private:
    osg::Vec3 _p;
    float _t;
};

}

#endif

// vim: set sw=4 ts=8 et ic ai:
