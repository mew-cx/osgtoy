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

/* file:      src/osgPlugins/globe/ReaderWriterGLOBE.cpp
 * author:    Mike Weiblen
 * copyright: (C) 2004-2006 Michael Weiblen http://mew.cx/
 * license:   OpenSceneGraph Public License (OSGPL)
 * $Id$
*/

#include <osg/Notify>
#include <osg/ShapeDrawable>
#include <osg/Texture2D>
#include <osg/Geode>

#include <osgDB/ReaderWriter>
#include <osgDB/FileNameUtils>
#include <osgDB/Registry>
#include <osgDB/ReadFile>

#include <stdio.h>

#define EXTENSION_NAME "globe"

///////////////////////////////////////////////////////////////////////////

/**
 * An OSG reader plugin for the ".globe" pseudo-loader, which loads an image
 * file, creates a sphere, then renders the image as a texture on that sphere.
 *
 * This loader makes it simple to render cylindrical maps (such as those from
 * http://www.johnstonsarchive.net/spaceart/planetcylmaps.html) as planets
 * or globes.
 *
 * Usage: <imagefile.ext>.<radius>.globe
 * where:
 *      <imagefile.ext> = an image filename.
 *      <radius> = radius for the globe.
 *
 * example: osgviewer land_shallow_topo_2048.jpg.10.globe
 */

class ReaderWriterGLOBE : public osgDB::ReaderWriter
{
public:
    ReaderWriterGLOBE()
    {
        supportsExtension( EXTENSION_NAME, "globe pseudo-loader" );
    }
    
    const char* className() const { return "globe pseudo-loader"; }

    ReadResult readNode(const std::string& fileName, const osgDB::ReaderWriter::Options* /*options*/) const
    {
        std::string ext( osgDB::getLowerCaseFileExtension(fileName) );
        if( !acceptsExtension(ext) )
            return ReadResult::FILE_NOT_HANDLED;

        osg::notify(osg::INFO) << "ReaderWriterGLOBE( \"" << fileName << "\" )" << std::endl;

        // strip the ".globe" pseudo-loader extension
        std::string tmpName( osgDB::getNameLessExtension(fileName) );

        // get the next "extension", which actually contains the globe radius parameter
        std::string params( osgDB::getFileExtension(tmpName) );
        if( params.empty() )
        {
            osg::notify(osg::WARN) << "Missing parameters for " EXTENSION_NAME " pseudo-loader" << std::endl;
            return ReadResult::FILE_NOT_HANDLED;
        }

        // strip the "params extension", which must leave an image subfilename.
        std::string subFileName( osgDB::getNameLessExtension(tmpName) );
        if( subFileName.empty() || subFileName == tmpName )
        {
            osg::notify(osg::WARN) << "Missing image subfilename for " EXTENSION_NAME " pseudo-loader" << std::endl;
            return ReadResult::FILE_NOT_HANDLED;
        }

        osg::notify(osg::INFO) << EXTENSION_NAME " params = \"" << params << "\"" << std::endl;

        int radius;
        int count( sscanf( params.c_str(), "%d", &radius ) );
        if( count != 1 )
        {
            osg::notify(osg::WARN) << "Bad parameters for " EXTENSION_NAME " pseudo-loader: \"" << params << "\"" << std::endl;
            return ReadResult::FILE_NOT_HANDLED;
        }

        // recursively load the image subfile.
        osg::Image *image( osgDB::readImageFile(subFileName) );
        if( !image )
        {
            // propagate the read failure upwards
            osg::notify(osg::WARN) << "Image file \"" << subFileName << "\" could not be loaded" << std::endl;
            return ReadResult::FILE_NOT_HANDLED;
        }

        // create an osg::Sphere for the globe geometry
        osg::Geode* geode( new osg::Geode() );
        geode->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0,0,0), radius)));

        // apply the image as a texture to the globe
        osg::Texture2D* tex2d( new osg::Texture2D );
        tex2d->setImage( image );
        tex2d->setWrap( osg::Texture::WRAP_S, osg::Texture::REPEAT );
        tex2d->setWrap( osg::Texture::WRAP_T, osg::Texture::REPEAT );

        osg::StateSet* stateset( geode->getOrCreateStateSet() );
        stateset->setTextureAttributeAndModes( 0, tex2d, osg::StateAttribute::ON );

        return geode;
    }
};

// Add ourself to the Registry to instantiate the reader/writer.
osgDB::RegisterReaderWriterProxy<ReaderWriterGLOBE> g_readerWriter_GLOBE_Proxy;

// vim: set sw=4 ts=8 et ic ai:
