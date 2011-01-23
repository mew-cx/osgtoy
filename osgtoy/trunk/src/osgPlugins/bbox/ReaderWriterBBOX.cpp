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

/* file:      src/osgPlugins/bbox/ReaderWriterBBOX.cpp
 * author:    Mike Weiblen http://mew.cx/ 2011-01-11
 * copyright: (C) 2011 Michael Weiblen
 * license:   OpenSceneGraph Public License (OSGPL)
*/

#include <osg/Geode>
#include <osg/Notify>
#include <osg/ComputeBoundsVisitor>
#include <osg/io_utils>

#include <osgDB/ReaderWriter>
#include <osgDB/FileNameUtils>
#include <osgDB/Registry>
#include <osgDB/ReadFile>

#include <osgToy/Geometry.h>

#include <stdio.h>

#define EXTENSION_NAME "bbox"


class ReaderWriterBBOX : public osgDB::ReaderWriter
{
public:
    ReaderWriterBBOX() { }
    
    virtual const char* className() const { return EXTENSION_NAME " pseudo-loader"; }

    virtual bool acceptsExtension( const std::string& extension ) const
    { 
        return osgDB::equalCaseInsensitive( extension, EXTENSION_NAME );
    }

    void usage() const
    {
        osg::notify(osg::NOTICE)
            << "\nUsage for " << className() << ":\n"
            << "<subfile>." EXTENSION_NAME "\n\tdecorate scene with wireframe bounding box\n"
            << std::endl;
    }

    virtual ReadResult readNode( const std::string& fileName, const osgDB::ReaderWriter::Options* options ) const
    {
        std::string ext( osgDB::getLowerCaseFileExtension(fileName) );
        if( ! acceptsExtension(ext) )
            return ReadResult::FILE_NOT_HANDLED;

        osg::notify(osg::INFO) << "ReaderWriterBBOX( \"" << fileName << "\" )" << std::endl;

        // remove the pseudo-loader extension
        std::string subfile( osgDB::getNameLessExtension( fileName ) );
        if( subfile.empty() )
        {
            osg::notify(osg::WARN) << "Missing subfile for " << className() << std::endl;
            return ReadResult::FILE_NOT_HANDLED;
        }

        // recursively load the subfile.
        osg::Node* scene( osgDB::readNodeFile( subfile, options ) );
        if( !scene )
        {
            // propagate the read failure upwards
            osg::notify(osg::WARN) << "Subfile \"" << subfile << "\" could not be loaded" << std::endl;
            return ReadResult::FILE_NOT_HANDLED;
        }


        osg::ComputeBoundsVisitor cbbv;
        scene->accept(cbbv);
        const osg::BoundingBox& bbox( cbbv.getBoundingBox() );
    
        osg::notify(osg::NOTICE)
            << "\nBounding box of \"" << subfile << "\":"
            << "\n\tmin: " << bbox._min
            << "\n\tmax: " << bbox._max
            << std::endl;

        osg::Geode* geode( new osg::Geode );
        geode->addDrawable( new osgToy::WireBox( bbox ) );

        osg::Group* root( new osg::Group );
        root->addChild( scene );
        root->addChild( geode );

        return root;
    }
};


// Add ourself to the Registry to instantiate the reader/writer.
REGISTER_OSGPLUGIN( bbox, ReaderWriterBBOX )

// vim: set sw=4 ts=8 et ic ai:
