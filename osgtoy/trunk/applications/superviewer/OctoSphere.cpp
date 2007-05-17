// OctoSphere.cpp
// mew 2006-04-26

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Vec3>
#include <osg/Vec4>

//#define USE_CUBEMAP
#ifdef USE_CUBEMAP //[
#include <osg/StateSet>
#include "svUtil.h"
#endif //]

///////////////////////////////////////////////////////////////////////////

static osg::Vec4
color( osg::Vec3 v )
{
    return osg::Vec4( (v + osg::Vec3(1,1,1)) / 2., 1 );
}

static osg::Vec3Array* _vAry;
static osg::Vec4Array* _cAry;
#ifdef USE_CUBEMAP //[
static osg::Vec3Array* _tAry;
#endif //]

static void addTriangle( const osg::Vec3& v0, const osg::Vec3& v1, const osg::Vec3& v2 )
{
    _vAry->push_back( v0 ); _vAry->push_back( v1 ); _vAry->push_back( v2 );
#ifdef USE_CUBEMAP //[
    _tAry->push_back( v0 ); _tAry->push_back( v1 ); _tAry->push_back( v2 );
#endif //]

    //_cAry->push_back( color(v0) ); _cAry->push_back( color(v1) ); _cAry->push_back( color(v2) );
}

static void
newTriangle( int level, const osg::Vec3& v0, const osg::Vec3& v1, const osg::Vec3& v2 )
{
    if( level <= 0 )
    {
        addTriangle( v0, v1, v2 );
        return;
    }

    level--;
#if 0 //[
    // this face subdivision is WRONG, but it sure looks cool
    osg::Vec3 vc = v0 + v1 + v2; vc.normalize();
    newTriangle( level, v0, v1, vc );
    newTriangle( level, v0, vc, v2 );
    newTriangle( level, vc, v1, v2 );
#else
    // this edge subdivision is correct.
    osg::Vec3 va = v0 + v1; va.normalize();
    osg::Vec3 vb = v1 + v2; vb.normalize();
    osg::Vec3 vc = v0 + v2; vc.normalize();
    newTriangle( level, v0, va, vc );
    newTriangle( level, v1, vb, va );
    newTriangle( level, v2, vc, vb );

    // comment out this line for a sierpinski mesh
    //newTriangle( level, va, vb, vc );
#endif //]
}

///////////////////////////////////////////////////////////////////////////

osg::Node* makeOctoSphere( unsigned int level )
{
    osg::Geometry* geom = new osg::Geometry;

    _vAry = new osg::Vec3Array;
    geom->setVertexArray( _vAry );

    geom->setNormalArray( _vAry );
    geom->setNormalBinding( osg::Geometry::BIND_PER_VERTEX );

    _cAry = new osg::Vec4Array;
    geom->setColorArray( _cAry );
    geom->setColorBinding( osg::Geometry::BIND_OVERALL );
    _cAry->push_back( osg::Vec4(1,1,1,1) );

#ifdef USE_CUBEMAP //[
    _tAry = new osg::Vec3Array;
    geom->setTexCoordArray( 0, _tAry );
#endif //]

    osg::Vec3 xp( 1, 0, 0);
    osg::Vec3 xn(-1, 0, 0);
    osg::Vec3 yp( 0, 1, 0);
    osg::Vec3 yn( 0,-1, 0);
    osg::Vec3 zp( 0, 0, 1);
    osg::Vec3 zn( 0, 0,-1);

    newTriangle( level, xp, yp, zp );   // +++
    newTriangle( level, yn, xp, zp );   // +-+
    newTriangle( level, yp, xn, zp );   // -++
    newTriangle( level, xn, yn, zp );   // --+

    newTriangle( level, xp, yp, zn );   // ++-
    newTriangle( level, yn, xp, zn );   // +--
    newTriangle( level, yp, xn, zn );   // -+-
    newTriangle( level, xn, yn, zn );   // ---

    geom->addPrimitiveSet( new osg::DrawArrays( GL_TRIANGLES, 0, _vAry->size() ) );

#ifdef USE_CUBEMAP //[
    osg::StateSet* ss = geom->getOrCreateStateSet();
    ss->setTextureAttributeAndModes( 0,
            svUtil::readCubeMap( "Cubemap_test/", ".png" ),
            osg::StateAttribute::ON );
#endif //]

    osg::Geode* geode = new osg::Geode();
    geode->addDrawable( geom );
    return geode;
}

// vim: set sw=4 ts=8 et ic ai:
