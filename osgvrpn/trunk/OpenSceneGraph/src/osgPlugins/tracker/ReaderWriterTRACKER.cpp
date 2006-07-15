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

/* file:        src/osgPlugins/tracker/ReaderWriterTRACKER.cpp
 * author:      Mike Weiblen
 * copyright:   (C) 2004-2006  Mike Weiblen http://mew.cx/
 * license:     OpenSceneGraph Public License (OSGPL)
 * $Id: ReaderWriterTRACKER.cpp,v 1.3 2006/07/15 17:28:35 mew Exp $
*/

#include <osg/Notify>

#include <osgVRPN/TrackerBase.h>
#include <osgVRPN/Tracker.h>
#include <osgVRPN/TrackerTransform.h>

#include <osgDB/ReaderWriter>
#include <osgDB/FileNameUtils>
#include <osgDB/Registry>
#include <osgDB/ReadFile>

#define EXTENSION_NAME "tracker"

///////////////////////////////////////////////////////////////////////////

/**
 * An OSG reader plugin for the osgVRPN tracker pseudo-loader.
 */
class ReaderWriterTRACKER : public osgDB::ReaderWriter
{
public:
    ReaderWriterTRACKER() { }
    
    const char* className() const { return "osgVRPN tracker pseudo-loader"; }

    bool acceptsExtension(const std::string& extension) const
    { 
        return osgDB::equalCaseInsensitive( extension, EXTENSION_NAME );
    }

    ReadResult readNode(const std::string& fileName,
                const osgDB::ReaderWriter::Options* /*options*/) const
    {
        std::string ext = osgDB::getLowerCaseFileExtension(fileName);
        if( !acceptsExtension(ext) )
            return ReadResult::FILE_NOT_HANDLED;

        osg::notify(osg::INFO) << "ReaderWriterTRACKER( \"" << fileName << "\" )" << std::endl;

        // strip the pseudo-loader extension
        std::string tmpName = osgDB::getNameLessExtension( fileName );

        // get the next "extension", which actually the osgVRPN tracker name
        std::string trkName = osgDB::getFileExtension( tmpName );
        if( trkName.empty() )
        {
            osg::notify(osg::WARN) << "Missing trkName for " EXTENSION_NAME " pseudo-loader" << std::endl;
            return ReadResult::FILE_NOT_HANDLED;
        }

        // strip the trkName "extension", which must leave a sub-filename.
        std::string subFileName = osgDB::getNameLessExtension( tmpName );
        if( subFileName == tmpName )
        {
            osg::notify(osg::WARN) << "Missing subfilename for " EXTENSION_NAME " pseudo-loader" << std::endl;
            return ReadResult::FILE_NOT_HANDLED;
        }

        // recursively load the subfile.
        osg::Node *node = osgDB::readNodeFile( subFileName );
        if( !node )
        {
            // propagate the read failure upwards
            osg::notify(osg::WARN) << "Subfile \"" << subFileName << "\" could not be loaded" << std::endl;
            return ReadResult::FILE_NOT_HANDLED;
        }

        // TODO option to select the kind of Tracker?

        osgVRPN::TrackerBase* tracker = 0;
        tracker = new osgVRPN::Tracker( trkName.c_str() );
        osgVRPN::TrackerTransform *xform( new osgVRPN::TrackerTransform(tracker) );
        xform->addChild( node );
        return xform;
    }
};


// Add ourself to the Registry to instantiate the reader/writer.
osgDB::RegisterReaderWriterProxy<ReaderWriterTRACKER> g_readerWriter_TRACKER_Proxy;

// vim: set sw=4 ts=8 et ic ai:
