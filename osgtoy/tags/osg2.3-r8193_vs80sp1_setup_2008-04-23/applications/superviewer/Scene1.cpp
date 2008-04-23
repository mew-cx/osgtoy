/* file:        applications/superviewer/Scene1.cpp
 * author:      Mike Weiblen 2005-09-20
 *
 * This is basically the origin of "glsl_simple.osg"
*/

#include <osg/ShapeDrawable>
#include <osg/PositionAttitudeTransform>
#include <osg/Geode>
#include <osg/Node>
#include <osg/Notify>
#include <osg/Vec3>
#include <osg/Program>
#include <osg/Shader>
#include <osg/Uniform>
#include "svUtil.h"

///////////////////////////////////////////////////////////////////////////

static osg::Group* rootNode;

// Create some geometry upon which to render GLSL shaders.
static osg::Geode*
CreateModel()
{
    osg::Drawable* drawable;

    osg::Geode* geode = new osg::Geode();

    drawable = new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0,0,0),1));
    drawable->getOrCreateStateSet()->addUniform( new osg::Uniform( "Color1", osg::Vec3(0.2,0.8,0) ) );
    geode->addDrawable( drawable );

    drawable = new osg::ShapeDrawable(new osg::Cone(osg::Vec3(2.2,0,-0.4),0.9,1.8));
    drawable->getOrCreateStateSet()->addUniform( new osg::Uniform( "Color1", osg::Vec3(0.9,0.2,0.2) ) );
    geode->addDrawable( drawable );

    drawable = new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(-2.2,0,0),1,1.4));
    drawable->getOrCreateStateSet()->addUniform( new osg::Uniform( "Color1", osg::Vec3(0.2,0,0.8) ) );
    geode->addDrawable( drawable );

    return geode;
}

// Add a reference to the masterModel at the specified translation, and
// return its StateSet so we can easily attach StateAttributes.
static osg::StateSet*
ModelInstance()
{
    static float zvalue = 0;
    static osg::Node* masterModel = CreateModel();

    osg::PositionAttitudeTransform* xform = new osg::PositionAttitudeTransform();
    xform->setPosition(osg::Vec3(0,2,zvalue));
    zvalue = zvalue + 2.2;
    xform->addChild(masterModel);
    rootNode->addChild(xform);
    return xform->getOrCreateStateSet();
}

///////////////////////////////////////////////////////////////////////////
// Compose a scenegraph with examples of GLSL shaders

osg::Node* makeScene1()
{
    // the root of our scenegraph.
    rootNode = new osg::Group;

    // attach some Uniforms to the root, to be inherited by Programs.
    if(1){
        osg::StateSet* ss = rootNode->getOrCreateStateSet();
        ss->addUniform( new osg::Uniform( "Sine", 0.5f ) );
        ss->addUniform( new osg::Uniform( "Color1", osg::Vec3(0.5f, 1.0f, 1.0f) ) );
        ss->addUniform( new osg::Uniform( "Color2", osg::Vec3(1.0f, 0.5f, 0.0f) ) );
    }

    // simple Microshader
    {
        osg::StateSet* ss = ModelInstance();
        osg::Program* pgm = new osg::Program;
        ss->setAttributeAndModes( pgm, osg::StateAttribute::ON );
        pgm->addShader( new osg::Shader( osg::Shader::VERTEX, 
            "void main(void)\n"
            "{\n"
            "    gl_FrontColor = gl_Vertex;\n"
            "    gl_Position   = ftransform();\n"
            "}\n" ) );
    }


    // the "blocky" shader, a simple animation test
    {
        osg::StateSet* ss = ModelInstance();
        osg::Program* pgm = svUtil::GlslProgramManager::instance().createSimpleFileProgram( "shaders/blocky" );
        ss->setAttributeAndModes( pgm, osg::StateAttribute::ON );
    }

    return rootNode;
}

// vim: set sw=4 ts=8 et ic ai:
