/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2005 Robert Osfield 
 *
 * This application is open source and may be redistributed and/or modified   
 * freely and without restriction, both in commericial and non commericial applications,
 * as long as this copyright notice is maintained.
 * 
 * This application is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

/* file:        applications/superviewer/Utils.cpp
 * author:      Mike Weiblen
 * copyright:   (C) 2005 Michael Weiblen
 * license:     OpenSceneGraph Public License (OSGPL)
 * website:     http://mew.cx/
 * $Id 2006-04-26 $
*/

#include <osg/Notify>
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>
#include <osgGA/GUIEventAdapter>

#include "svUtil.h"

///////////////////////////////////////////////////////////////////////////

osg::Program*
svUtil::createMicroshaderProgram()
{
    osg::Program* pgm = new osg::Program;
    pgm->setName( "microshader" );

    pgm->addShader( new osg::Shader( osg::Shader::VERTEX,
        "varying vec4 color;\n"
        "void main(void)\n"
        "{\n"
        "    color = vec4( gl_Vertex.rgb, 1. );\n"
        "    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
        "}\n" ));

    pgm->addShader( new osg::Shader( osg::Shader::FRAGMENT,
        "varying vec4 color;\n"
        "void main(void)\n"
        "{\n"
        "    gl_FragColor = color;\n"
        "}\n" ));

    return pgm;
}


void svUtil::readShaderSource( osg::Shader* shader )
{
    // we expect osg::Shader name to contain actual and existing fully-
    // qualified name of the text file containing the source code.
    std::string fqFileName = shader->getName();
    if( ! fqFileName.empty() )
        shader->loadShaderSourceFromFile( fqFileName );
    else
        shader->setShaderSource( "ERROR - no source file!!!" );
}

void svUtil::readProgramSources( osg::Program* program )
{
    for( unsigned int i = 0; i < program->getNumShaders(); ++i )
    {
        readShaderSource( program->getShader( i ) );
    }
}


std::string svUtil::createSimpleFileName(
        osg::Shader::Type type, const char* baseName )
{
    std::string fileName = baseName;
    fileName += (type == osg::Shader::VERTEX) ? ".vert" : ".frag";
    return fileName;
}

std::string svUtil::createSimpleFqFileName(
        osg::Shader::Type type, const char* baseName )
{
    std::string fileName = createSimpleFileName( type, baseName );
    std::string fqFileName = osgDB::findDataFile( fileName );
    if( fqFileName.empty() )
        osg::notify(osg::WARN) << "File \"" << fileName << "\" not found." << std::endl;
    return fqFileName;
}


osg::Shader*
svUtil::createShaderFromFile( osg::Shader::Type type, const char* baseName )
{
    osg::Shader* shader = new osg::Shader( type );
    shader->setName( createSimpleFqFileName(type, baseName) );
    readShaderSource( shader );
    return shader;
}

///////////////////////////////////////////////////////////////////////////

osg::TextureCubeMap*
svUtil::readCubeMap( const std::string prefix, const std::string suffix )
{
    osg::TextureCubeMap* cubemap = new osg::TextureCubeMap;

    cubemap->setImage(osg::TextureCubeMap::POSITIVE_X, osgDB::readImageFile( prefix+"posx"+suffix ));
    cubemap->setImage(osg::TextureCubeMap::NEGATIVE_X, osgDB::readImageFile( prefix+"negx"+suffix ));
    cubemap->setImage(osg::TextureCubeMap::POSITIVE_Y, osgDB::readImageFile( prefix+"posy"+suffix ));
    cubemap->setImage(osg::TextureCubeMap::NEGATIVE_Y, osgDB::readImageFile( prefix+"negy"+suffix ));
    cubemap->setImage(osg::TextureCubeMap::POSITIVE_Z, osgDB::readImageFile( prefix+"posz"+suffix ));
    cubemap->setImage(osg::TextureCubeMap::NEGATIVE_Z, osgDB::readImageFile( prefix+"negz"+suffix ));

    cubemap->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
    cubemap->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
    cubemap->setWrap(osg::Texture::WRAP_R, osg::Texture::CLAMP_TO_EDGE);

    cubemap->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR);
    cubemap->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);

    return cubemap;
}

///////////////////////////////////////////////////////////////////////////

svUtil::GlslProgramManager&
svUtil::GlslProgramManager::instance()
{
    static GlslProgramManager s_instance;
    return s_instance;
}

osg::Program*
svUtil::GlslProgramManager::createSimpleFileProgram( const char* baseName )
{
    osg::Program* pgm = new osg::Program;
    pgm->setName( baseName );
    pgm->addShader( createShaderFromFile( osg::Shader::VERTEX, baseName ) );
    pgm->addShader( createShaderFromFile( osg::Shader::FRAGMENT, baseName ) );
    addProgram( pgm );
    return pgm;
}

void
svUtil::GlslProgramManager::addProgram( osg::ref_ptr<osg::Program> program )
{
    _programList.push_back(program);
}

void
svUtil::GlslProgramManager::readAllProgramSources()
{
    for( ProgramList::iterator i = _programList.begin(); i != _programList.end(); ++i )
    {
        readProgramSources( (*i).get() );
    }
}

///////////////////////////////////////////////////////////////////////////

svUtil::EventHandler::EventHandler() {}
    
bool svUtil::EventHandler::handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
{
    switch( ea.getEventType() )
    {
        case osgGA::GUIEventAdapter::SCROLL_UP:
            osg::notify(osg::INFO) << "SCROLL_UP" << std::endl;
            break;

        case osgGA::GUIEventAdapter::SCROLL_DOWN:
            osg::notify(osg::INFO) << "SCROLL_DOWN" << std::endl;
            break;

        case osgGA::GUIEventAdapter::SCROLL_LEFT:
            osg::notify(osg::INFO) << "SCROLL_LEFT" << std::endl;
            break;

        case osgGA::GUIEventAdapter::SCROLL_RIGHT:
            osg::notify(osg::INFO) << "SCROLL_RIGHT" << std::endl;
            break;

        case osgGA::GUIEventAdapter::KEYDOWN:
            switch( ea.getKey() )
            {
                case 'x':
                    // reload all shader sources
                    svUtil::GlslProgramManager::instance().readAllProgramSources();
                    return true;

                case 'y':
                    // TODO toggle osg::Program enable state
                    return true;

                case osgGA::GUIEventAdapter::KEY_Left:
                    osg::notify(osg::INFO) << "KEY_Left" << std::endl;
                    break;

                case osgGA::GUIEventAdapter::KEY_Right:
                    osg::notify(osg::INFO) << "KEY_Right" << std::endl;
                    break;

                case osgGA::GUIEventAdapter::KEY_Up:
                    osg::notify(osg::INFO) << "KEY_Up" << std::endl;
                    break;

                case osgGA::GUIEventAdapter::KEY_Down:
                    osg::notify(osg::INFO) << "KEY_Down" << std::endl;
                    break;

                case osgGA::GUIEventAdapter::KEY_Page_Up:
                    osg::notify(osg::INFO) << "KEY_Page_Up" << std::endl;
                    break;

                case osgGA::GUIEventAdapter::KEY_Page_Down:
                    osg::notify(osg::INFO) << "KEY_Page_Down" << std::endl;
                    break;

                case osgGA::GUIEventAdapter::KEY_Home:
                    osg::notify(osg::INFO) << "KEY_Home" << std::endl;
                    break;

                case osgGA::GUIEventAdapter::KEY_End:
                    osg::notify(osg::INFO) << "KEY_End" << std::endl;
                    break;
            }
            break;

        default:
            break;
    }
    return false;
}

// vim: set sw=4 ts=8 et ic ai: 
