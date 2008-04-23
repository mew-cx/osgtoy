/* file:        applications/superviewer/Mipmap.cpp
 * author:      Mike Weiblen 2005-11-26
*/

#include <osg/Node>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Vec2>
#include <osg/Vec3>
#include <osg/Vec4>
#include <osg/Texture2D>

#include <osgToy/MipmapTestImage>

///////////////////////////////////////////////////////////////////////////

osg::Node* makeMipmapScene()
{
    const int TEXUNIT = 0;

    osg::Geometry* geom = new osg::Geometry;

    osg::Vec3Array* vAry = new osg::Vec3Array;
    geom->setVertexArray( vAry );

    osg::Vec2Array* tAry = new osg::Vec2Array;
    geom->setTexCoordArray( TEXUNIT, tAry );

    osg::Vec3Array* nAry = new osg::Vec3Array;
    geom->setNormalArray( nAry );
    geom->setNormalBinding( osg::Geometry::BIND_OVERALL );
    nAry->push_back( osg::Vec3(0,-1,0) );

    osg::Vec4Array* cAry = new osg::Vec4Array;
    geom->setColorArray( cAry );
    geom->setColorBinding( osg::Geometry::BIND_OVERALL );
    cAry->push_back( osg::Vec4(1,1,1,1) );

    vAry->push_back( osg::Vec3(0,0,0) ); tAry->push_back( osg::Vec2(0,0) );
    vAry->push_back( osg::Vec3(1,0,0) ); tAry->push_back( osg::Vec2(1,0) );
    vAry->push_back( osg::Vec3(0,0,1) ); tAry->push_back( osg::Vec2(0,1) );
    vAry->push_back( osg::Vec3(1,0,1) ); tAry->push_back( osg::Vec2(1,1) );

    geom->addPrimitiveSet( new osg::DrawArrays(
                GL_TRIANGLE_STRIP, 0, vAry->getNumElements() ) );

    osg::Texture2D* tex = new osg::Texture2D;
    tex->setImage( new osgToy::MipmapTestImage( 128, 512 ) );

    osg::StateSet* ss = geom->getOrCreateStateSet();
    ss->setTextureAttributeAndModes( TEXUNIT, tex, osg::StateAttribute::ON );

    osg::Geode* geode = new osg::Geode();
    geode->addDrawable( geom );
    return geode;
}

// vim: set sw=4 ts=8 et ic ai: 
