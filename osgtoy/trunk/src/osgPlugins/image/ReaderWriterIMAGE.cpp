/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2005 Robert Osfield 
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

/* file:      src/osgPlugins/image/ReaderWriterIMAGE.cpp
 * author:    Mike Weiblen
 * copyright: (C) 2005-2006 Michael Weiblen http://mew.cx/
 * license:   OpenSceneGraph Public License (OSGPL)
 * $Id$
*/

#include <osg/Notify>
#include <osg/Image>

#include <osgDB/ReaderWriter>
#include <osgDB/FileNameUtils>
#include <osgDB/Registry>
#include <osgDB/ReadFile>

#define EXTENSION_NAME "image"

///////////////////////////////////////////////////////////////////////////

/**
 * An OSG reader plugin for the ".image" pseudo-loader, which loads an
 * image file and renders it on a quad.
 *
 * Usage: <file.ext>.image
 *
 * example: osgviewer horsetooth.png.image
 */

class ReaderWriterIMAGE : public osgDB::ReaderWriter
{
public:
    ReaderWriterIMAGE()
    {
        supportsExtension( EXTENSION_NAME, "image pseudo-loader" );
    }
    
    const char* className() const { return "image pseudo-loader"; }

    ReadResult readNode(const std::string& fileName, const osgDB::ReaderWriter::Options* options) const
    {
        std::string ext( osgDB::getLowerCaseFileExtension(fileName) );
        if( !acceptsExtension(ext) )
            return ReadResult::FILE_NOT_HANDLED;

        osg::notify(osg::INFO) << "ReaderWriterIMAGE( \"" << fileName << "\" )" << std::endl;

        // strip the pseudo-loader extension, which must leave a sub-filename.
        std::string subFileName( osgDB::getNameLessExtension( fileName ) );
        if( subFileName == fileName )
        {
            osg::notify(osg::WARN) << "Missing subfilename for " EXTENSION_NAME " pseudo-loader" << std::endl;
            return ReadResult::FILE_NOT_HANDLED;
        }

        // recursively load the subfile.
        osg::Image* image( readImageFile(subFileName.c_str(), options) );
        if (!image)
        {
            // propagate the read failure upwards
            osg::notify(osg::WARN) << "Subfile \"" << subFileName << "\" could not be loaded" << std::endl;
            return ReadResult::FILE_NOT_HANDLED;
        }

        return osg::createGeodeForImage(image);
    }
};

// Add ourself to the Registry to instantiate the reader/writer.
osgDB::RegisterReaderWriterProxy<ReaderWriterIMAGE> g_readerWriter_IMAGE_Proxy;

// vim: set sw=4 ts=8 et ic ai:
