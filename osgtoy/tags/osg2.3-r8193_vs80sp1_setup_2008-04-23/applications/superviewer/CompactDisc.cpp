/* file:        applications/superviewer/CompactDisc.cpp
 * author:      Mike Weiblen 2005-09-20
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
#include <osg/Program>
#include <osg/Shader>
#include <osg/Uniform>
#include <osg/MatrixTransform>
#include <osgDB/FileUtils>
#include <osgGA/GUIEventHandler>
#include <osgProducer/Viewer>
#include "svUtil.h"

// Compact Disc physical dimensions [meters]
// http://en.wikipedia.org/wiki/Compact_disc
// http://www.disctronics.co.uk/technology/cdbasics/cd_specs.htm
const float CD_DISC_RADIUS = 60.0e-3;
const float CD_PGM_OUTER_RADIUS = 58.0e-3;
const float CD_PGM_INNER_RADIUS = 23.0e-3;
const float CD_HOLE_RADIUS = 7.5e-3;
const float CD_DISC_THICKNESS = 1.2e-3;
const float CD_PIT_WIDTH = 500.0e-9;
const float CD_PIT_DEPTH = 150.0e-9;
const float CD_PIT_LEN_MIN = 800.0e-9;
const float CD_PIT_LEN_MAX = 3500.0e-9;
const float CD_TRACK_PITCH = 1600.0e-9;

// WORKAROUND: due to shortcomings of some GLSL implementation, pass
// tangent vector in a texcoord rather than a generic attribute.
#define USE_VERTEX_ATTRIB
const int TANGENT_ATTRIB_INDEX = 5;

///////////////////////////////////////////////////////////////////////////

static osg::Geode*
buildDiffractiveDisc( float rInner, float rOuter )
{
    osg::Geometry* geom = new osg::Geometry;

    // vertex array
    osg::Vec3Array* vertex = new osg::Vec3Array;
    geom->setVertexArray( vertex );

    // color array
    osg::Vec4Array* color = new osg::Vec4Array;
    geom->setColorArray( color );
    geom->setColorBinding( osg::Geometry::BIND_OVERALL );
    color->push_back( osg::Vec4(1,1,1,1) );

    // normal array
    osg::Vec3Array* normal = new osg::Vec3Array;
    geom->setNormalArray( normal );
    geom->setNormalBinding( osg::Geometry::BIND_OVERALL );
    normal->push_back( osg::Vec3(0,0,1) );

    // tangent array
    osg::Vec3Array* tangent = new osg::Vec3Array;
#ifdef USE_VERTEX_ATTRIB //[
    geom->setVertexAttribArray( TANGENT_ATTRIB_INDEX, tangent );
    geom->setVertexAttribBinding( TANGENT_ATTRIB_INDEX, osg::Geometry::BIND_PER_VERTEX );
#else
    geom->setTexCoordArray( 0, tangent );
#endif //]

    for( int i=0; i<=360; ++i )
    {
        float a = osg::DegreesToRadians( (float)i );
        float x = cosf(a);
        float y = sinf(a);

        vertex->push_back( osg::Vec3( rInner*x, rInner*y, 0 ) );
        tangent->push_back( osg::Vec3( -y, x, 0 ) );

        vertex->push_back( osg::Vec3( rOuter*x, rOuter*y, 0 ) );
        tangent->push_back( osg::Vec3( -y, x, 0 ) );
    }

    geom->addPrimitiveSet( new osg::DrawArrays( GL_TRIANGLE_STRIP, 0, vertex->getNumElements() ) );

    osg::Geode* geode = new osg::Geode;
    geode->addDrawable( geom );
    return geode;
}

///////////////////////////////////////////////////////////////////////////
// create a white circle with unit radius.

static osg::Geode*
buildCircle()
{
    osg::Geometry* geom = new osg::Geometry;

    // vertex array
    osg::Vec3Array* vertex = new osg::Vec3Array;
    geom->setVertexArray( vertex );

    // color array
    osg::Vec4Array* color = new osg::Vec4Array;
    geom->setColorArray( color );
    geom->setColorBinding( osg::Geometry::BIND_OVERALL );
    color->push_back( osg::Vec4(1,1,1,1) );

    for( int i=0; i<360; ++i )
    {
        float a = osg::DegreesToRadians( (float)i );
        float x = cosf(a);
        float y = sinf(a);
        vertex->push_back( osg::Vec3( x, y, 0 ) );
    }
    geom->addPrimitiveSet( new osg::DrawArrays( GL_LINE_LOOP, 0, 360 ) );

    osg::Geode* geode = new osg::Geode;
    geode->addDrawable( geom );
    return geode;
}

///////////////////////////////////////////////////////////////////////////

osg::Node* makeCompactDiscScene()
{
    osg::Group* root = new osg::Group;
    osg::StateSet* ss = root->getOrCreateStateSet();
    ss->setMode( GL_LIGHTING, osg::StateAttribute::OFF );

    osg::Geode* disc = buildDiffractiveDisc( CD_PGM_INNER_RADIUS, CD_PGM_OUTER_RADIUS );
    root->addChild( disc );
    {
        osg::StateSet* ss = disc->getOrCreateStateSet();

        osg::Program* pgm = new osg::Program;
        pgm->setName( "diffraction_glsl" );
        svUtil::GlslProgramManager::instance().addProgram(pgm);
        pgm->addShader( svUtil::createShaderFromFile( osg::Shader::VERTEX, "diffraction_glsl" ) );
        ss->setAttributeAndModes( pgm, osg::StateAttribute::ON );

#ifdef USE_VERTEX_ATTRIB //[
        pgm->addBindAttribLocation( "Tangent", TANGENT_ATTRIB_INDEX );
#endif //]

        ss->addUniform( new osg::Uniform( "SurfaceRoughness", 30.0f ) );
        ss->addUniform( new osg::Uniform( "GratingSpacing", CD_TRACK_PITCH * 1.0e9f ) );
        ss->addUniform( new osg::Uniform( "HighlightColor", osg::Vec3(1,1,1) ) );
    }

    osg::Geode* circle = buildCircle();

    // a circle for the center hole of the disc
    osg::MatrixTransform* xform = new osg::MatrixTransform;
    xform->setDataVariance( osg::Object::STATIC );
    xform->setMatrix( osg::Matrix::scale( CD_HOLE_RADIUS, CD_HOLE_RADIUS, CD_HOLE_RADIUS ) );
    xform->addChild( circle );
    root->addChild( xform );

    // a circle for the outer edge of the disc
    xform = new osg::MatrixTransform;
    xform->setDataVariance( osg::Object::STATIC );
    xform->setMatrix( osg::Matrix::scale( CD_DISC_RADIUS, CD_DISC_RADIUS, CD_DISC_RADIUS ) );
    xform->addChild( circle );
    root->addChild( xform );

    return root;
}

// vim: set sw=4 ts=8 et ic ai:
