/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2004 Robert Osfield 
 *
 * This application is open source and may be redistributed and/or modified   
 * freely and without restriction, both in commericial and non commericial
 * applications, as long as this copyright notice is maintained.
 * 
 * This application is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
*/

/* file:        src/osgPlugins/lorenz/ReaderWriterLORENZ.cpp
 * author:      Mike Weiblen 2005-05-03
 * copyright:   (C) 2004-2005 Michael Weiblen
 * license:     OpenSceneGraph Public License (OSGPL)
 * website:     http://mew.cx/osg/
 * $Id: ReaderWriterLORENZ.cpp,v 1.1 2005/06/06 22:10:37 mew Exp $
*/

#include <stdio.h>
#include <sstream>

#include <osg/Notify>
#include <osg/Geode>
#include <osgDB/ReaderWriter>
#include <osgDB/FileNameUtils>
#include <osgDB/Registry>
#include <osgDB/ReadFile>

#include <osgToy/LorenzAttractor>

#define EXTENSION_NAME "lorenz"

///////////////////////////////////////////////////////////////////////////

/**
 * An OSG reader plugin for the Lorenz attractor pseudo-loader.
 *
 * Usage: <numPoints>.lorenz
 * where:
 *      <numPoints> = number of points in Lorenz Attractor [integer]
 *
 * example: osgviewer 10000.lorenz
 */

class ReaderWriterLORENZ : public osgDB::ReaderWriter
{
public:
    ReaderWriterLORENZ() {}
    virtual ~ReaderWriterLORENZ() {}
    
    virtual const char* className() const { return "Lorenz attractor pseudo-loader"; }

    virtual bool acceptsExtension(const std::string& extension) const
    {
        return osgDB::equalCaseInsensitive( extension, EXTENSION_NAME );
    }

    virtual ReadResult readNode( const std::string&,
                const osgDB::ReaderWriter::Options* ) const;
};

///////////////////////////////////////////////////////////////////////////

osgDB::ReaderWriter::ReadResult
ReaderWriterLORENZ::readNode( const std::string& fileName,
                const osgDB::ReaderWriter::Options* options ) const
{
    std::string ext = osgDB::getLowerCaseFileExtension(fileName);
    if( !acceptsExtension(ext) )
        return ReadResult::FILE_NOT_HANDLED;

    osg::notify(osg::INFO) << "ReaderWriterLORENZ( \"" << fileName << "\" )" << std::endl;

    // strip the pseudo-loader extension, leaving the parameter string
    std::string params = osgDB::getNameLessExtension( fileName );

    osg::notify(osg::INFO) << EXTENSION_NAME " params = \"" << params << "\"" << std::endl;

    int numPoints;
    int count = sscanf( params.c_str(), "%d", &numPoints );
    if( count != 1 )
    {
        osg::notify(osg::WARN)
                << "Bad parameters for " EXTENSION_NAME " pseudo-loader: \""
                << params << "\"" << std::endl;
        return ReadResult::FILE_NOT_HANDLED;
    }

    osg::Geode* geode = new osg::Geode;
    geode->addDrawable( new osgToy::LorenzAttractor( numPoints ) );
    return geode;
}


// Add ourself to the Registry to instantiate the reader/writer.
osgDB::RegisterReaderWriterProxy<ReaderWriterLORENZ> g_readerWriter_LORENZ_Proxy;

/* vim: set sw=4 ts=8 et ic ai: */
/*EOF*/
