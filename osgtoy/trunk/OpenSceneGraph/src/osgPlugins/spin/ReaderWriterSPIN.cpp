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

/* file:      src/osgPlugins/spin/ReaderWriterSPIN.cpp
 * author:    Mike Weiblen
 * copyright: (C) 2004-2006 Michael Weiblen http://mew.cx/
 * license:   OpenSceneGraph Public License (OSGPL)
 * $Id: ReaderWriterSPIN.cpp,v 1.2 2006/07/15 23:57:44 mew Exp $
*/
// TODO: should spinAxis & angularVelcity be pseudoloader parameters?

#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osgUtil/TransformCallback>

#include <osgDB/ReaderWriter>
#include <osgDB/FileNameUtils>
#include <osgDB/Registry>
#include <osgDB/ReadFile>

#include <stdio.h>

#define EXTENSION_NAME "spin"

///////////////////////////////////////////////////////////////////////////

/**
 * An OSG reader plugin for the ".spin" pseudo-loader, which inserts an
 * animated rotation transform above the loaded geometry.
 *
 * example: osgviewer cow.osg.spin
 */

class ReaderWriterSPIN : public osgDB::ReaderWriter
{
public:
    ReaderWriterSPIN() { }
    
    const char* className() const { return "spin animation pseudo-loader"; }

    bool acceptsExtension(const std::string& extension) const
    { 
        return osgDB::equalCaseInsensitive( extension, EXTENSION_NAME );
    }

    ReadResult readNode(const std::string& fileName, const osgDB::ReaderWriter::Options* options) const
    {
        std::string ext( osgDB::getLowerCaseFileExtension(fileName) );
        if( !acceptsExtension(ext) )
            return ReadResult::FILE_NOT_HANDLED;

        osg::notify(osg::INFO) << "ReaderWriterSPIN( \"" << fileName << "\" )" << std::endl;

        // strip the pseudo-loader extension
        std::string subFileName( osgDB::getNameLessExtension(fileName) );

        if( subFileName == fileName )
        {
            osg::notify(osg::WARN) << "Missing subfilename for " EXTENSION_NAME " pseudo-loader" << std::endl;
            return ReadResult::FILE_NOT_HANDLED;
        }

        // recursively load the subfile.
        osg::Node *node( osgDB::readNodeFile( subFileName, options ) );
        if( !node )
        {
            // propagate the read failure upwards
            osg::notify(osg::WARN) << "Subfile \"" << subFileName << "\" could not be loaded" << std::endl;
            return ReadResult::FILE_NOT_HANDLED;
        }

        // Are these worth making pseudoloader parameters?
        osg::Vec3 spinAxis( osg::Vec3(0,0,1) );
        float angularVelocity( osg::inDegrees(60.0f) );  // [per second]

        // add a transform with animation callback to spin the loaded model.
        osg::MatrixTransform* spinXform( new osg::MatrixTransform );
        spinXform->addChild( node );
        osg::NodeCallback* nc( new osgUtil::TransformCallback( spinXform->getBound().center(),
                spinAxis, angularVelocity ) );
        spinXform->setUpdateCallback( nc );
        return spinXform;
    }
};

// Add ourself to the Registry to instantiate the reader/writer.
osgDB::RegisterReaderWriterProxy<ReaderWriterSPIN> g_readerWriter_SPIN_Proxy;

// vim: set sw=4 ts=8 et ic ai:
