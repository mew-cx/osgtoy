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

/* file:      src/osgPlugins/smoother/ReaderWriterSMOOTHER.cpp
 * author:    Mike Weiblen
 * copyright: (C) 2004-2006 Michael Weiblen http://mew.cx/
 * license:   OpenSceneGraph Public License (OSGPL)
 * $Id$
*/

#include <osg/Notify>
#include <osg/Node>
#include <osg/StateSet>

#include <osgDB/ReaderWriter>
#include <osgDB/FileNameUtils>
#include <osgDB/Registry>
#include <osgDB/ReadFile>

#include <osgUtil/SmoothingVisitor>

#define EXTENSION_NAME "smoother"

///////////////////////////////////////////////////////////////////////////

/**
 * An OSG reader plugin for the ".smoother" pseudo-loader, which runs
 * osgUtil::SmoothingVisitor on the loaded model.
 *
 * Usage: <modelfile.ext>.smoother
 * where:
 *      <modelfile.ext> = an model filename.
 *
 * example: osgviewer vase.osg.smoother
 */

class ReaderWriterSMOOTHER : public osgDB::ReaderWriter
{
public:
    ReaderWriterSMOOTHER()
    {
        supportsExtension( EXTENSION_NAME, "SmoothingVisitor pseudo-loader" );
    }
    
    const char* className() const { return "SmoothingVisitor pseudo-loader"; }

    ReadResult readNode(const std::string& fileName,
                const osgDB::ReaderWriter::Options* /*options*/) const
    {
        std::string ext( osgDB::getLowerCaseFileExtension(fileName) );
        if( !acceptsExtension(ext) )
            return ReadResult::FILE_NOT_HANDLED;

        osg::notify(osg::INFO) << "ReaderWriterSMOOTHER( \"" << fileName << "\" )" << std::endl;

        // strip the pseudo-loader extension
        std::string subFileName( osgDB::getNameLessExtension(fileName) );
        if( subFileName == fileName )
        {
            osg::notify(osg::WARN) << "Missing subfilename for " EXTENSION_NAME " pseudo-loader" << std::endl;
            return ReadResult::FILE_NOT_HANDLED;
        }

        // recursively load the subfile.
        osg::Node *node( osgDB::readNodeFile(subFileName) );
        if( !node )
        {
            // propagate the read failure upwards
            osg::notify(osg::WARN) << "Subfile \"" << subFileName << "\" could not be loaded" << std::endl;
            return ReadResult::FILE_NOT_HANDLED;
        }

        osgUtil::SmoothingVisitor sv;
        node->accept( sv );

        osg::StateSet* sset( node->getOrCreateStateSet() );
        sset->setMode( GL_LIGHTING, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE );

        return node;
    }
};

// Add ourself to the Registry to instantiate the reader/writer.
osgDB::RegisterReaderWriterProxy<ReaderWriterSMOOTHER> g_readerWriter_SMOOTHER_Proxy;

// vim: set sw=4 ts=8 et ic ai:
