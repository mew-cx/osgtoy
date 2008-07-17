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

/* file:      src/osgPlugins/osgviewer/ReaderWriterOSGVIEWER.cpp
 * author:    Mike Weiblen
 * copyright: (C) 2005-2007 Michael Weiblen http://mew.cx/
 * license:   OpenSceneGraph Public License (OSGPL)
 * $Id$
*/

#include <osg/Notify>
#include <osg/Image>

#include <osgDB/ReaderWriter>
#include <osgDB/FileNameUtils>
#include <osgDB/Registry>
#include <osgDB/ReadFile>

#define EXTENSION_NAME "osgviewer"

///////////////////////////////////////////////////////////////////////////

/**
 * An OSG reader plugin for the ".osgviewer" pseudo-loader.
 * This pseudo-loader is a hack for Windows: it simply strips off its
 * extension, then tries to load the subfilename as osg::Node or
 * osg::Image.
 * The idea is that the Windows Explorer file association can be bound
 * to the ".osgviewer" extension, which will view the file in the
 * standard osgviewer.exe application.
 *
 * Usage: <file.ext>.osgviewer
 *
 * example: start cow.osg.osgviewer
 * example: start abasin.png.osgviewer
 */

class ReaderWriterOSGVIEWER : public osgDB::ReaderWriter
{
public:
    ReaderWriterOSGVIEWER() { }
    {
        supportsExtension( EXTENSION_NAME, "osgviewer pseudo-loader" );
    }
    
    const char* className() const { return "osgviewer pseudo-loader"; }

    ReadResult readNode(const std::string& fileName, const osgDB::ReaderWriter::Options* options) const
    {
        std::string ext( osgDB::getLowerCaseFileExtension(fileName) );
        if( !acceptsExtension(ext) )
            return ReadResult::FILE_NOT_HANDLED;

        osg::notify(osg::INFO) << "ReaderWriterOSGVIEWER( \"" << fileName << "\" )" << std::endl;

        // strip the pseudo-loader extension, which must leave a sub-filename.
        std::string subFileName( osgDB::getNameLessExtension( fileName ) );
        if( subFileName == fileName )
        {
            osg::notify(osg::WARN) << "Missing subfilename for " EXTENSION_NAME " pseudo-loader" << std::endl;
            return ReadResult::FILE_NOT_HANDLED;
        }

        // recursively load the subfile.
        // first try to load as osg::Node
        osg::Node* node( osgDB::readNodeFile( subFileName, options ) );
        if( node )
        {
            return node;
        }

        // else try to load as osg::Image, which will be applied to a quad.
        osg::Image* image( readImageFile(subFileName.c_str(), options) );
        if( image )
        {
            return osg::createGeodeForImage(image);
        }

        // propagate the read failure upwards
        osg::notify(osg::WARN) << "Subfile \"" << subFileName << "\" could not be loaded" << std::endl;
        return ReadResult::FILE_NOT_HANDLED;
    }
};

// Add ourself to the Registry to instantiate the reader/writer.
osgDB::RegisterReaderWriterProxy<ReaderWriterOSGVIEWER> g_readerWriter_OSGVIEWER_Proxy;

// vim: set sw=4 ts=8 et ic ai:
