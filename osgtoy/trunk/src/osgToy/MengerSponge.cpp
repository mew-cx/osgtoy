/* file:        src/osgToy/MengerSponge.cpp
 * author:      Mike Weiblen
 * copyright:   (C) 2004-2005 Michael Weiblen
 * license:     OpenSceneGraph Public License (OSGPL)
 * website:     http://mew.cx/
 * $Id$
*/

#include <osg/Notify>
#include <osgToy/MengerSponge.h>

enum {  // face flags (e.g. "NX" = negative X axis orientation)
    NX = 0x01,
    PX = 0x02,
    NY = 0x04,
    PY = 0x08,
    NZ = 0x10,
    PZ = 0x20
};

void osgToy::MengerSponge::addQuad( const osg::Vec3& v0, const osg::Vec3& v1,
        const osg::Vec3& v2, const osg::Vec3& v3 )
{
    osg::Vec3 normal = (v1-v0) ^ (v3-v0);
    normal.normalize();
    _nAry->push_back( normal );

    _numQuads++;
    _vAry->push_back( v0 );
    _vAry->push_back( v1 );
    _vAry->push_back( v2 );
    _vAry->push_back( v3 );
}

void osgToy::MengerSponge::subdivide( int level,
    float x0, float x3, float y0, float y3, float z0, float z3, int faces )
{
    if( faces == 0 ) return;

    if( level > 0 )
    {
        // recurse next subdivision level
        level--;

        float xd = (x3 - x0) / 3;
        float x1 = x0 + xd;
        float x2 = x3 - xd;

        float yd = (y3 - y0) / 3;
        float y1 = y0 + yd;
        float y2 = y3 - yd;

        float zd = (z3 - z0) / 3;
        float z1 = z0 + zd;
        float z2 = z3 - zd;

        // cube corners
        subdivide( level, x0, x1, y0, y1, z0, z1, faces & (NX|NY|NZ) );
        subdivide( level, x0, x1, y2, y3, z0, z1, faces & (NX|PY|NZ) );
        subdivide( level, x0, x1, y0, y1, z2, z3, faces & (NX|NY|PZ) );
        subdivide( level, x0, x1, y2, y3, z2, z3, faces & (NX|PY|PZ) );

        subdivide( level, x2, x3, y0, y1, z2, z3, faces & (PX|NY|PZ) );
        subdivide( level, x2, x3, y2, y3, z0, z1, faces & (PX|PY|NZ) );
        subdivide( level, x2, x3, y0, y1, z0, z1, faces & (PX|NY|NZ) );
        subdivide( level, x2, x3, y2, y3, z2, z3, faces & (PX|PY|PZ) );

        // cube edges
        subdivide( level, x0, x1, y1, y2, z0, z1, faces & (NX|NZ) | PX|PZ );
        subdivide( level, x1, x2, y0, y1, z0, z1, faces & (NY|NZ) | PY|PZ );
        subdivide( level, x1, x2, y2, y3, z0, z1, faces & (PY|NZ) | NY|PZ );
        subdivide( level, x2, x3, y1, y2, z0, z1, faces & (PX|NZ) | NX|PZ );

        subdivide( level, x0, x1, y0, y1, z1, z2, faces & (NX|NY) | PX|PY );
        subdivide( level, x0, x1, y2, y3, z1, z2, faces & (NX|PY) | PX|NY );
        subdivide( level, x2, x3, y0, y1, z1, z2, faces & (PX|NY) | NX|PY );
        subdivide( level, x2, x3, y2, y3, z1, z2, faces & (PX|PY) | NX|NY );

        subdivide( level, x0, x1, y1, y2, z2, z3, faces & (NX|PZ) | PX|NZ );
        subdivide( level, x1, x2, y0, y1, z2, z3, faces & (NY|PZ) | PY|NZ );
        subdivide( level, x1, x2, y2, y3, z2, z3, faces & (PY|PZ) | NY|NZ );
        subdivide( level, x2, x3, y1, y2, z2, z3, faces & (PX|PZ) | NX|NZ );
    }
    else
    {
        // subdivision complete, build face geometry

        if( faces & NX )
            addQuad(
                osg::Vec3(x0, y0, z0),
                osg::Vec3(x0, y0, z3),
                osg::Vec3(x0, y3, z3),
                osg::Vec3(x0, y3, z0) );

        if( faces & PX )
            addQuad(
                osg::Vec3(x3, y0, z0),
                osg::Vec3(x3, y3, z0),
                osg::Vec3(x3, y3, z3),
                osg::Vec3(x3, y0, z3) );

        if( faces & NY )
            addQuad(
                osg::Vec3(x0, y0, z0),
                osg::Vec3(x3, y0, z0),
                osg::Vec3(x3, y0, z3),
                osg::Vec3(x0, y0, z3) );

        if( faces & PY )
            addQuad(
                osg::Vec3(x0, y3, z0),
                osg::Vec3(x0, y3, z3),
                osg::Vec3(x3, y3, z3),
                osg::Vec3(x3, y3, z0) );

        if( faces & NZ )
            addQuad(
                osg::Vec3(x0, y0, z0),
                osg::Vec3(x0, y3, z0),
                osg::Vec3(x3, y3, z0),
                osg::Vec3(x3, y0, z0) );

        if( faces & PZ )
            addQuad(
                osg::Vec3(x0, y0, z3),
                osg::Vec3(x3, y0, z3),
                osg::Vec3(x3, y3, z3),
                osg::Vec3(x0, y3, z3) );
    }
}


osgToy::MengerSponge::MengerSponge( unsigned int level, float scale ) :
    _numQuads(0)
{
    _vAry = new osg::Vec3Array;
    setVertexArray( _vAry );

    _nAry = new osg::Vec3Array;
    setNormalArray( _nAry );
    setNormalBinding( osg::Geometry::BIND_PER_PRIMITIVE );

    osg::Vec4Array* cAry = new osg::Vec4Array;
    setColorArray( cAry );
    setColorBinding( osg::Geometry::BIND_OVERALL );
    cAry->push_back( osg::Vec4(1,1,1,1) );

    float d = scale / 2;
    subdivide( level, -d, d, -d, d, -d, d, NX|PX|NY|PY|NZ|PZ );

    addPrimitiveSet( new osg::DrawArrays( GL_QUADS, 0, _vAry->size() ) );

    osg::notify(osg::INFO)
            << "Menger sponge level=" << level
            << " generated " << getNumQuads() << " quads"
            << std::endl;
}

// vim: set sw=4 ts=8 et ic ai:
