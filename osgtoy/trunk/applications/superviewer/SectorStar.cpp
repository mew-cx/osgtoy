/* file:   applications/superviewer/SectorStar.cpp
 * author: Mike Weiblen
 * $Id: SectorStar.cpp,v 1.1 2007/02/17 05:00:57 mew Exp $
*/

#include <map>
#include <osg/Referenced>
#include <osg/ref_ptr>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Node>
#include <osg/Math>
#include <osg/Notify>
#include <osg/Vec3>
#include <osg/Vec4>
#include <osgProducer/Viewer>


///////////////////////////////////////////////////////////////////////////

static osg::Geode*
buildMoire( float increment = 1.0f )
{
    osg::Geometry* geom = new osg::Geometry;

    // color array
    osg::Vec4Array* color = new osg::Vec4Array;
    geom->setColorArray( color );
    geom->setColorBinding( osg::Geometry::BIND_OVERALL );
    color->push_back( osg::Vec4(1,1,1,1) );

    // vertex array
    osg::Vec3Array* vertex = new osg::Vec3Array;
    geom->setVertexArray( vertex );

    for( float i=0; i<=360; i+=increment )
    {
        float a = osg::DegreesToRadians( i - increment/4. );
        float x = cosf(a);
        float y = sinf(a);
        vertex->push_back( osg::Vec3( x, 0, y ) );

        a = osg::DegreesToRadians( i + increment/4. );
        x = cosf(a);
        y = sinf(a);
        vertex->push_back( osg::Vec3( x, 0, y ) );

        vertex->push_back( osg::Vec3( 0, 0, 0 ) );
    }

    geom->addPrimitiveSet( new osg::DrawArrays( GL_TRIANGLES, 0, vertex->getNumElements() ) );

    osg::Geode* geode = new osg::Geode;
    geode->addDrawable( geom );
    return geode;
}

///////////////////////////////////////////////////////////////////////////

osg::Node* makeSectorStarScene()
{
    osg::Group* root = new osg::Group;
    osg::StateSet* ss = root->getOrCreateStateSet();
    ss->setMode( GL_LIGHTING, osg::StateAttribute::OFF );

    root->addChild( buildMoire(0.5) );
    return root;
}

// vim: set sw=4 ts=8 et ic ai:
