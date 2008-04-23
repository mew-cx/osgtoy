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

/* file:        applications/superviewer/svUtil.h
 * author:      Mike Weiblen
 * copyright:   (C) 2005 Michael Weiblen
 * license:     OpenSceneGraph Public License (OSGPL)
 * website:     http://mew.cx/
 * $Id 2005-10-17 $
*/

#ifndef SVUTIL_H
#define SVUTIL_H

#include <string>
#include <osg/Program>
#include <osg/Shader>
#include <osg/TextureCubeMap>
#include <osgGA/GUIEventHandler>
#include <osgGA/GUIEventAdapter>

namespace svUtil {

osg::Program* createMicroshaderProgram();

void readShaderSource( osg::Shader* shader );
void readProgramSources( osg::Program* program );

std::string createSimpleFileName( osg::Shader::Type type, const char* baseName );
std::string createSimpleFqFileName( osg::Shader::Type type, const char* baseName );

osg::Shader* createShaderFromFile( osg::Shader::Type type, const char* baseName );

///////////////////////////////////////////////////////////////////////////

osg::TextureCubeMap* readCubeMap( const std::string prefix, const std::string suffix );

///////////////////////////////////////////////////////////////////////////

class GlslProgramManager
{
public:
    static GlslProgramManager& instance();

    osg::Program* createSimpleFileProgram( const char* baseName );
    void addProgram( osg::ref_ptr<osg::Program> program );
    void readAllProgramSources();

protected:
    GlslProgramManager() {}
    ~GlslProgramManager() {}

    typedef std::vector< osg::ref_ptr< osg::Program > > ProgramList;
    ProgramList _programList;
};

///////////////////////////////////////////////////////////////////////////

class EventHandler : public osgGA::GUIEventHandler
{
public:
    EventHandler();
    bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& );
};

}

#endif

// vim: set sw=4 ts=8 et ic ai:
