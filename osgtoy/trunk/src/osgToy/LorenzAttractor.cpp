/* file:        src/osgToy/LorenzAttractor.cpp
 * author:      Mike Weiblen 2005-05-25
 * copyright:   (C) 2004-2005 Michael Weiblen
 * license:     OpenSceneGraph Public License (OSGPL)
 * website:     http://mew.cx/
 * $Id$
*/

#include <osg/Notify>
#include <osg/Vec3>

#include <osgToy/RungeKutta4.h>
#include <osgToy/LorenzAttractor.h>

///////////////////////////////////////////////////////////////////////////

class LorenzSolver : public osgToy::RungeKutta4
{
public:
    LorenzSolver( osg::Vec3 p0, float t0, float a, float b, float c ) :
            RungeKutta4( p0, t0 ), _a(a), _b(b), _c(c) {}

private:
    osg::Vec3 dpdt( const osg::Vec3& p, float t )
    {
        return osg::Vec3(
            _a * (p.y() - p.x()),
            p.x() * (_b - p.z()) - p.y(),
            p.x() * p.y() - _c * p.z() );
    }
    float _a, _b, _c;
};

///////////////////////////////////////////////////////////////////////////

osgToy::LorenzAttractor::LorenzAttractor( unsigned int numPoints,
        float deltaT, osg::Vec3 p0, float t0, float a, float b, float c ) :
    _numPoints( numPoints )
{
    osg::Vec3Array* vAry = new osg::Vec3Array;
    setVertexArray( vAry );

    osg::Vec4Array* cAry = new osg::Vec4Array;
    setColorArray( cAry );
    setColorBinding( osg::Geometry::BIND_OVERALL );
    cAry->push_back( osg::Vec4(1,1,1,1) );

    LorenzSolver lor( p0, t0, a, b, c );
    vAry->reserve( _numPoints );
    for( unsigned int i = 0; i < _numPoints; ++i )
    {
        vAry->push_back( lor.p() );
        lor.advance( deltaT );
    }

    addPrimitiveSet( new osg::DrawArrays( GL_POINTS, 0, vAry->size() ) );

    osg::StateSet* sset = getOrCreateStateSet();
    sset->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
}

// vim: set sw=4 ts=8 et ic ai:
