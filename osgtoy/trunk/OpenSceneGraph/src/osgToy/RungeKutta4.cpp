/* file:        src/osgToy/RungeKutta4.cpp
 * author:      Mike Weiblen 2004-12-21
 * copyright:   (C) 2004-2005 Michael Weiblen
 * license:     OpenSceneGraph Public License (OSGPL)
 * website:     http://mew.cx/osg/
 * $Id: RungeKutta4.cpp,v 1.2 2006/09/11 06:24:05 mew Exp $
*/

#include <osgToy/RungeKutta4>

void osgToy::RungeKutta4::advance( float h )
{
    float h2 = h / 2.0f;
    osg::Vec3 k0 = dpdt( _p, _t );
    osg::Vec3 k1 = dpdt( _p+k0*h2, _t+h2 );
    osg::Vec3 k2 = dpdt( _p+k1*h2, _t+h2 );
    osg::Vec3 k3 = dpdt( _p+k2*h, _t+h );
    _p += (k0 + (k1 + k2) * 2.0f + k3) * (h / 6.0f);
    _t += h;
}

// vim: set sw=4 ts=8 et ic ai:
