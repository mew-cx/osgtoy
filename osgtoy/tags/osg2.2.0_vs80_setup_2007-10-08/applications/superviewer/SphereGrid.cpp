/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2005 Robert Osfield 
 * Copyright (C) 2005 3Dlabs Inc. Ltd.
 *
 * This application is open source and may be redistributed and/or modified   
 * freely and without restriction, both in commericial and non commericial applications,
 * as long as this copyright notice is maintained.
 * 
 * This application is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

/* file:        applications/superviewer/SphereGrid.cpp
 * author:      Mike Weiblen
 * copyright:   (C) 2005 Michael Weiblen
 * license:     OpenSceneGraph Public License (OSGPL)
 * website:     http://mew.cx/
 * $Id 2005-10-17 $
*/

#include <osg/ShapeDrawable>
#include <osg/PositionAttitudeTransform>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Node>
#include <osg/Notify>
#include <osg/Vec3>
#include <osg/Program>
#include <osg/Shader>
#include <osg/Uniform>
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>

#include "svUtil.h"

///////////////////////////////////////////////////////////////////////////

static void
attachShader( osg::Node* node, int shaderNum )
{
    osg::StateSet* ss = node->getOrCreateStateSet();
    switch( shaderNum )
    {
        case 0:
        {
            //osg::Program* pgm = svUtil::createMicroshaderProgram();
            osg::Program* pgm = svUtil::GlslProgramManager::instance().createSimpleFileProgram("microshader");
            ss->setAttributeAndModes( pgm, osg::StateAttribute::ON );
            break;
        }

        case 1:
        {
            const char* name = "blocky2";
            osg::Program* pgm = new osg::Program;
            pgm->setName( name );
            pgm->addShader( svUtil::createShaderFromFile( osg::Shader::VERTEX, "LightPosition" ) );
            pgm->addShader( svUtil::createShaderFromFile( osg::Shader::VERTEX, "DiffuseLighting" ) );
            //pgm->addShader( svUtil::createShaderFromFile( osg::Shader::VERTEX, "SpecularLighting" ) );
            pgm->addShader( svUtil::createShaderFromFile( osg::Shader::VERTEX, name ) );
            pgm->addShader( svUtil::createShaderFromFile( osg::Shader::FRAGMENT, name ) );
            ss->setAttributeAndModes( pgm, osg::StateAttribute::ON );
            svUtil::GlslProgramManager::instance().addProgram(pgm);
            break;
        }

        case 2:
        {
            const char* name = "brick2";
            osg::Program* pgm = new osg::Program;
            pgm->setName( name );
            pgm->addShader( svUtil::createShaderFromFile( osg::Shader::VERTEX, "LightPosition" ) );
            pgm->addShader( svUtil::createShaderFromFile( osg::Shader::VERTEX, "SpecularLighting" ) );
            pgm->addShader( svUtil::createShaderFromFile( osg::Shader::VERTEX, name ) );
            pgm->addShader( svUtil::createShaderFromFile( osg::Shader::FRAGMENT, name ) );
            ss->setAttributeAndModes( pgm, osg::StateAttribute::ON );
            svUtil::GlslProgramManager::instance().addProgram(pgm);
            break;
        }

        case 3:
        {
            const char* name = "screen2";
            osg::Program* pgm = new osg::Program;
            pgm->setName( name );
            pgm->addShader( svUtil::createShaderFromFile( osg::Shader::VERTEX, name ) );
            pgm->addShader( svUtil::createShaderFromFile( osg::Shader::FRAGMENT, name ) );
            ss->setAttributeAndModes( pgm, osg::StateAttribute::ON );
            svUtil::GlslProgramManager::instance().addProgram(pgm);
            break;
        }

        default:
            break;
    }
}

///////////////////////////////////////////////////////////////////////////

osg::Node* makeSphereGrid( int width, int height, int depth )
{
    osg::Group* rootNode = new osg::Group;

    osg::Geode* model = new osg::Geode();
    model->addDrawable(new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(0,0,0),1) ));

    osg::Group* parent = rootNode;
    for( int d = 0; d < depth; ++d )
        for( int h = 0; h < height; ++h )
            for( int w = 0; w < width; ++w )
            {
                osg::PositionAttitudeTransform* xform = new osg::PositionAttitudeTransform();
                xform->setPosition( osg::Vec3(w,d,h) * 2.2 );
                xform->addChild( model );
                parent->addChild( xform );
            }

    return rootNode;
}

///////////////////////////////////////////////////////////////////////////

osg::Node* makeShadedSpheres()
{
    int numSpheres = 5;

    osg::Group* rootNode = new osg::Group;
    {
        osg::StateSet* ss = rootNode->getOrCreateStateSet();
        //ss->addUniform( new osg::Uniform( "LightPosition", osg::Vec3(0,0,4) ) );
        //ss->addUniform( new osg::Uniform( "Color2", osg::Vec3(0,1,0) ) );
    }

    osg::Geode* model = new osg::Geode();
    model->addDrawable(new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(0,0,0),1) ));

    osg::Group* parent = rootNode;
    for( int i = 0; i < numSpheres; ++i )
    {
        osg::PositionAttitudeTransform* xform = new osg::PositionAttitudeTransform();
        xform->addChild( model );

        parent->addChild( xform );

#if 0 //[
        // absolute positioning of peers
        xform->setPosition( osg::Vec3(i,0,0) * 2.2 );
#else
        // relative positioning of children
        xform->setPosition( osg::Vec3(1,0,0) * 2.2 );
        parent = xform;
#endif //]

        attachShader( xform, i );
    }

    return rootNode;
}

// vim: set sw=4 ts=8 et ic ai:
