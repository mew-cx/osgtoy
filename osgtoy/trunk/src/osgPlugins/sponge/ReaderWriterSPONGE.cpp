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

/* file:      src/osgPlugins/sponge/ReaderWriterSPONGE.cpp
 * author:    Mike Weiblen
 * copyright: (C) 2004-2006 Michael Weiblen http://mew.cx/
 * license:   OpenSceneGraph Public License (OSGPL)
 * $Id: ReaderWriterSPONGE.cpp,v 1.2 2006/07/15 23:57:44 mew Exp $
*/

#include <stdio.h>
#include <sstream>

#include <osg/Notify>
#include <osg/Geode>
#include <osgDB/ReaderWriter>
#include <osgDB/FileNameUtils>
#include <osgDB/Registry>
#include <osgDB/ReadFile>

#include <osgToy/MengerSponge.h>

#define EXTENSION_NAME "sponge"

///////////////////////////////////////////////////////////////////////////

/**
 * An OSG reader plugin for the Menger sponge pseudo-loader.
 *
 * Usage: <level>.sponge
 * where:
 *      <level> = recursion level of the Menger sponge (integer)
 *
 * example: osgviewer 3.sponge
 */

class ReaderWriterSPONGE : public osgDB::ReaderWriter
{
public:
    ReaderWriterSPONGE()
    {
        supportsExtension( EXTENSION_NAME, "Menger sponge pseudo-loader" );
    }

    const char* className() const { return "Menger sponge pseudo-loader"; }

    ReadResult readNode( const std::string&, const osgDB::ReaderWriter::Options* ) const;
};

///////////////////////////////////////////////////////////////////////////

osgDB::ReaderWriter::ReadResult
ReaderWriterSPONGE::readNode( const std::string& fileName,
                const osgDB::ReaderWriter::Options* options ) const
{
    std::string ext( osgDB::getLowerCaseFileExtension(fileName) );
    if( !acceptsExtension(ext) )
        return ReadResult::FILE_NOT_HANDLED;

    osg::notify(osg::INFO) << "ReaderWriterSPONGE( \"" << fileName << "\" )" << std::endl;

    // strip the pseudo-loader extension, leaving the parameter string
    std::string params( osgDB::getNameLessExtension( fileName ) );

    osg::notify(osg::INFO) << EXTENSION_NAME " params = \"" << params << "\"" << std::endl;

    int level;
    int count( sscanf( params.c_str(), "%d", &level ) );
    if( count != 1 )
    {
        osg::notify(osg::WARN)
                << "Bad parameters for " EXTENSION_NAME " pseudo-loader: \""
                << params << "\"" << std::endl;
        return ReadResult::FILE_NOT_HANDLED;
    }

    osg::Geode* geode( new osg::Geode );
    geode->addDrawable( new osgToy::MengerSponge( level ) );
    return geode;
}

// Add ourself to the Registry to instantiate the reader/writer.
osgDB::RegisterReaderWriterProxy<ReaderWriterSPONGE> g_readerWriter_SPONGE_Proxy;

// vim: set sw=4 ts=8 et ic ai:
