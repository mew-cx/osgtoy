/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2006 Robert Osfield 
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

/* file:      src/osgPlugins/cpp/ReaderWriterCPP.cpp
 * author:    Mike Weiblen http://mew.cx/
 * copyright: (C) 2007 Michael Weiblen
 * license:   OpenSceneGraph Public License (OSGPL)
 * $Id 2007-02-03 $
*/

#include <osg/Notify>
#include <osgDB/ReaderWriter>
#include <osgDB/FileNameUtils>
#include <osgDB/Registry>

#include "Image.h"

#define EXTENSION_NAME "cpp"

using namespace cpp;

class ReaderWriterCPP : public osgDB::ReaderWriter
{
public:
    ReaderWriterCPP() {}
    
    virtual const char* className() const { return "CPP source writer"; }

    virtual bool acceptsExtension(const std::string& extension) const
    { 
        return osgDB::equalCaseInsensitive( extension, EXTENSION_NAME );
    }

    virtual WriteResult writeImage( const osg::Image& image, std::ostream& fout, const osgDB::ReaderWriter::Options* options ) const
    {
        try
        {
            fout << image;
            return WriteResult::FILE_SAVED;
        }
        catch( ... )
        {
            osg::notify(osg::WARN) << EXTENSION_NAME << " plugin: error writing osg::Image" << std::endl;
        }
        return WriteResult::ERROR_IN_WRITING_FILE;
    }

    virtual WriteResult writeImage( const osg::Image& image, const std::string& fileName, const osgDB::ReaderWriter::Options* options ) const
    {
        std::string ext = osgDB::getFileExtension( fileName );
        if( !acceptsExtension(ext) ) return WriteResult::FILE_NOT_HANDLED;

        std::ofstream fout( fileName.c_str(), std::ios::out );
        if( !fout ) return WriteResult::ERROR_IN_WRITING_FILE;

        return writeImage( image, fout, options );
    }
};

// Add ourself to the Registry to instantiate the reader/writer.
osgDB::RegisterReaderWriterProxy<ReaderWriterCPP> g_readerWriter_CPP_Proxy;

// vim: set sw=4 ts=8 et ic ai:
