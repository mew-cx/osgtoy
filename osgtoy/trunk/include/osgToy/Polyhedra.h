/* file:        include/osgToy/Polyhedra
 * author:      Mike Weiblen
 * copyright:   (C) 2005 Mike Weiblen
 * license:     OpenSceneGraph Public License (OSGPL)
 * website:     http://mew.cx/
 * $Id: Polyhedra,v 1.1 2005/11/22 09:18:30 mew Exp $
*/

#ifndef OSGTOY_POLYHEDRA
#define OSGTOY_POLYHEDRA

#include <osg/Vec3>
#include <osg/Vec4>
#include <osg/Geometry>
#include <osgToy/Export.h>

namespace osgToy {

class OSGTOY_EXPORT Polyhedron : public osg::Geometry
{
public:
    Polyhedron();

protected:
    void setOverallColor( const osg::Vec4& color );
    void addTristrip(
            const osg::Vec3& u0, const osg::Vec3& u1,
            const osg::Vec3& v0, const osg::Vec3& v1,
            unsigned int numQuads=1 );
};

///////////////////////////////////////////////////////////////////////////

// http://mathworld.wolfram.com/PenroseTriangle.html
// http://en.wikipedia.org/wiki/Penrose_triangle
class OSGTOY_EXPORT PenroseTriangle : public osgToy::Polyhedron
{
public:
    PenroseTriangle();
};


// http://mathworld.wolfram.com/RhombicDodecahedron.html
// http://en.wikipedia.org/wiki/Rhombic_dodecahedron
class OSGTOY_EXPORT RhombicDodecahedron : public osgToy::Polyhedron
{
public:
    RhombicDodecahedron();
};


class OSGTOY_EXPORT ColorOriginTetra : public osgToy::Polyhedron
{
public:
    ColorOriginTetra();
private:
    void addTriangle( const osg::Vec3& v0, const osg::Vec3& v1, const osg::Vec3& v2 );

};


class OSGTOY_EXPORT OctoStrip : public osgToy::Polyhedron
{
public:
    OctoStrip();
};


class OSGTOY_EXPORT TetraStrip : public osgToy::Polyhedron
{
public:
    TetraStrip();
};

}

#endif

// vim: set sw=4 ts=8 et ic ai:
