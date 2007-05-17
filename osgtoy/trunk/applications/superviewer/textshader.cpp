/* file:        applications/superviewer/textshader.cpp
 * author:      Mike Weiblen 2005-10-06
*/

#include <osg/PositionAttitudeTransform>
#include <osg/ref_ptr>
#include <osg/Node>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Notify>
#include <osg/Vec2>
#include <osg/Vec3>
#include <osg/Vec4>
#include <osg/Texture2D>
#include <osg/Program>
#include <osg/Shader>
#include <osg/Uniform>
#include <osgDB/ReadFile>
#include "svUtil.h"

static const int GLYPHTEXUNIT = 0;

///////////////////////////////////////////////////////////////////////////

static
osg::Geode* makePointer(void)
{
    osg::Geometry* geom = new osg::Geometry;

    // vertices
    osg::Vec3Array* vAry = new osg::Vec3Array;
    geom->setVertexArray( vAry );

    // texcoords
    osg::Vec2Array* tAry = new osg::Vec2Array;
    geom->setTexCoordArray( GLYPHTEXUNIT, tAry );

    // colors
    osg::Vec4Array* cAry = new osg::Vec4Array;
    geom->setColorArray( cAry );
    geom->setColorBinding( osg::Geometry::BIND_OVERALL );
    cAry->push_back( osg::Vec4(1,1,1,1) );

    vAry->push_back( osg::Vec3( 1,0,2) );  tAry->push_back( osg::Vec2(1,1) );
    vAry->push_back( osg::Vec3(-1,0,2) );  tAry->push_back( osg::Vec2(0,1) );
    vAry->push_back( osg::Vec3( 1,0,1) );  tAry->push_back( osg::Vec2(1,0) );

    vAry->push_back( osg::Vec3(-1,0,1) );  tAry->push_back( osg::Vec2(0,0) );
    vAry->push_back( osg::Vec3( 1,0,1) );  tAry->push_back( osg::Vec2(1,0) );
    vAry->push_back( osg::Vec3(-1,0,2) );  tAry->push_back( osg::Vec2(0,1) );

    vAry->push_back( osg::Vec3( 0,0,0) );  tAry->push_back( osg::Vec2(0,0) );
    vAry->push_back( osg::Vec3( 1,0,1) );  tAry->push_back( osg::Vec2(0,0) );
    vAry->push_back( osg::Vec3(-1,0,1) );  tAry->push_back( osg::Vec2(0,0) );

    geom->addPrimitiveSet( new osg::DrawArrays( GL_TRIANGLES, 0, vAry->getNumElements() ) );

    osg::Geode* geode = new osg::Geode();
    geode->addDrawable( geom );
    return geode;
}

///////////////////////////////////////////////////////////////////////////

static
osg::Node* modelInstance( osg::Node* model, osg::Vec3 pos, const char* str )
{
    osg::PositionAttitudeTransform* xform = new osg::PositionAttitudeTransform;
    xform->setPosition( pos );
    xform->addChild( model );

    // IMPORTANT: incoming "str" must be at least 4 chars long!
    osg::Vec4 word( str[0], str[1], str[2], str[3] );
    xform->getOrCreateStateSet()->addUniform( new osg::Uniform( "Word", word ) );

    return xform;
}

///////////////////////////////////////////////////////////////////////////

static
void setupSceneResources( osg::StateSet* ss, osg::Vec2 cellSpan )
{
    // load font image as a texture
    osg::Texture2D* fontTex = new osg::Texture2D;
    fontTex->setImage( osgDB::readImageFile( "verasansmono.png" ) );

    // build the drawtext shader
    osg::Program* pgm = new osg::Program;
    pgm->setName( "drawtext" );
    svUtil::GlslProgramManager::instance().addProgram(pgm);
    pgm->addShader( svUtil::createShaderFromFile( osg::Shader::FRAGMENT, "drawtext" ) );

    ss->addUniform( new osg::Uniform( "GlyphTexSampler", GLYPHTEXUNIT ) );
    ss->addUniform( new osg::Uniform( "CellSpan", cellSpan ) );
    ss->setTextureAttributeAndModes( GLYPHTEXUNIT, fontTex, osg::StateAttribute::ON );
    ss->setAttributeAndModes( pgm, osg::StateAttribute::ON );
    ss->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
}

///////////////////////////////////////////////////////////////////////////

// 2 dataflags with gl_Fog info
osg::Node* makeDataflagSceneFog()
{
    osg::Group* group = new osg::Group;
    osg::StateSet* ss = group->getOrCreateStateSet();

    // load font image as a texture
    osg::Texture2D* fontTex = new osg::Texture2D;
    fontTex->setImage( osgDB::readImageFile( "verasansmono.png" ) );
    ss->setTextureAttributeAndModes( GLYPHTEXUNIT, fontTex, osg::StateAttribute::ON );
    ss->addUniform( new osg::Uniform( "GlyphTexSampler", GLYPHTEXUNIT ) );

    ss->addUniform( new osg::Uniform( "CellSpan", osg::Vec2(6,5) ) );
    ss->setMode( GL_LIGHTING, osg::StateAttribute::OFF );

    osg::Shader* drawtext = svUtil::createShaderFromFile( osg::Shader::FRAGMENT, "drawtext" );
    osg::Geode* pointer = makePointer();

    {
        osg::PositionAttitudeTransform* xform = new osg::PositionAttitudeTransform;
        osg::StateSet* ss = xform->getOrCreateStateSet();
        xform->setPosition( osg::Vec3(0,0,1) );
        xform->addChild( pointer );
        group->addChild( xform );

        const char* str = "Fog1";
        osg::Vec4 word( str[0], str[1], str[2], str[3] );
        ss->addUniform( new osg::Uniform( "Word", word ) );

        osg::Program* pgm = new osg::Program;
        pgm->setName( "fog1pgm" );
        pgm->addShader( drawtext );
        pgm->addShader( svUtil::createShaderFromFile( osg::Shader::FRAGMENT, "fogdata1" ) );
        ss->setAttributeAndModes( pgm, osg::StateAttribute::ON );
        svUtil::GlslProgramManager::instance().addProgram(pgm);
    }

    {
        osg::PositionAttitudeTransform* xform = new osg::PositionAttitudeTransform;
        osg::StateSet* ss = xform->getOrCreateStateSet();
        xform->setPosition( osg::Vec3(0,0,-1) );
        xform->addChild( pointer );
        group->addChild( xform );

        const char* str = "Fog2";
        osg::Vec4 word( str[0], str[1], str[2], str[3] );
        ss->addUniform( new osg::Uniform( "Word", word ) );

        osg::Program* pgm = new osg::Program;
        pgm->setName( "fog2pgm" );
        pgm->addShader( drawtext );
        pgm->addShader( svUtil::createShaderFromFile( osg::Shader::FRAGMENT, "fogdata2" ) );
        ss->setAttributeAndModes( pgm, osg::StateAttribute::ON );
        svUtil::GlslProgramManager::instance().addProgram(pgm);
    }

    return group;
}

osg::Node* makeDataflagScene99()
{
    const char* str =
        " !\"#$%&'()*+,-./0123456789:;<=>?"
        "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"
        "`abcdefghijklmnopqrstuvwxyz[|]~\177";
    const char* s = str;

    osg::Group* group = new osg::Group;
    osg::StateSet* ss = group->getOrCreateStateSet();
    setupSceneResources( ss, osg::Vec2(4,1) );

    osg::Geode* pointer = makePointer();
    for( int y=0; y<6; ++y )
        for( int x=0; x<4; ++x )
        {
            osg::Vec3 pos(3*x-6,0,3*y-9);
            group->addChild( modelInstance( pointer, pos, s ) );
            s += 4;
        }

    return group;
}

// build the scene with 3 copies of the dataflag
osg::Node* makeDataflagScene3()
{
    osg::Group* group = new osg::Group;
    osg::StateSet* ss = group->getOrCreateStateSet();
    setupSceneResources( ss, osg::Vec2(8,4) );

    osg::Geode* pointer = makePointer();
    group->addChild( modelInstance( pointer, osg::Vec3(-2.5,0,0), "Open" ) );
    group->addChild( modelInstance( pointer, osg::Vec3(0,0,0),    "GL &" ) );
    group->addChild( modelInstance( pointer, osg::Vec3(2.5,0,0),  "GLSL" ) );
    return group;
}

// build the scene with a single instance, to create glsl_dataflag.osg
osg::Node* makeDataflagScene1()
{
    osg::Geode* pointer = makePointer();
    osg::StateSet* ss = pointer->getOrCreateStateSet();
    setupSceneResources( ss, osg::Vec2(8,4) );
    return pointer;
}

// vim: set sw=4 ts=8 et ic ai: 
