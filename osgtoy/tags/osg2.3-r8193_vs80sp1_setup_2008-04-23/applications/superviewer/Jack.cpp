// Jack.cpp
// mew 2005-08-30

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Vec3>
#include <osg/StateSet>
#include <osg/Program>
#include <osg/Shader>
#include <osgDB/ReadFile>


///////////////////////////////////////////////////////////////////////////

static osg::Vec3Array* _vAry;
static osg::Vec3Array* _nAry;
static osg::Vec4Array* _cAry;

static osg::Vec4 color( osg::Vec3 v )
{
    return osg::Vec4( (v + osg::Vec3(1,1,1)) / 2., 1 );
}

static void addTriangle( const osg::Vec3& v0, const osg::Vec3& v1, const osg::Vec3& v2 )
{
    _vAry->push_back( v0 ); _vAry->push_back( v1 ); _vAry->push_back( v2 );
    _nAry->push_back( v0 ); _nAry->push_back( v1 ); _nAry->push_back( v2 );
    _cAry->push_back( color(v0) ); _cAry->push_back( color(v1) ); _cAry->push_back( color(v2) );
}

static void newTriangle( int level, const osg::Vec3& v0, const osg::Vec3& v1, const osg::Vec3& v2 )
{
    if( level > 0 )
    {
        level--;

        osg::Vec3 va = v0 + v1; va.normalize();
        osg::Vec3 vb = v1 + v2; vb.normalize();
        osg::Vec3 vc = v0 + v2; vc.normalize();

        newTriangle( level, v0, va, vc );
        newTriangle( level, v1, vb, va );
        newTriangle( level, v2, vc, vb );
        newTriangle( level, va, vb, vc );
    }
    else
    {
        addTriangle( v0, v1, v2 );
    }
}

osg::Node* makeJack()
{
    // ignore/override the level setting
    int level = 3;

    osg::Geometry* geom = new osg::Geometry;

    _vAry = new osg::Vec3Array;
    geom->setVertexArray( _vAry );

    _nAry = new osg::Vec3Array;
    geom->setNormalArray( _nAry );
    geom->setNormalBinding( osg::Geometry::BIND_PER_VERTEX );

    _cAry = new osg::Vec4Array;
    geom->setColorArray( _cAry );
    //geom->setNormalBinding( osg::Geometry::BIND_PER_VERTEX );
    geom->setColorBinding( osg::Geometry::BIND_OVERALL );
    _cAry->push_back( osg::Vec4(1,1,1,1) );

    osg::Vec3 px = osg::Vec3( 1, 0, 0);
    osg::Vec3 nx = osg::Vec3(-1, 0, 0);
    osg::Vec3 py = osg::Vec3( 0, 1, 0);
    osg::Vec3 ny = osg::Vec3( 0,-1, 0);
    osg::Vec3 pz = osg::Vec3( 0, 0, 1);
    osg::Vec3 nz = osg::Vec3( 0, 0,-1);

    newTriangle( level, px, py, pz );   // +++

    geom->addPrimitiveSet( new osg::DrawArrays( GL_TRIANGLES, 0, _vAry->size() ) );

    osg::StateSet* ss = geom->getOrCreateStateSet();

    //geom->dirtyDisplayList();
    //ss->setMode( GL_LIGHTING, osg::StateAttribute::OFF );

    osg::Geode* geode = new osg::Geode();
    geode->addDrawable( geom );
    return geode;
}

// vim: set sw=4 ts=8 et ic ai:
