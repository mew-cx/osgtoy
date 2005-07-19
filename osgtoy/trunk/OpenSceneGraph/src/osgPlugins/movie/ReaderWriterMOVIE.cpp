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

/* file:	src/osgPlugins/movie/ReaderWriterMOVIE.cpp
 * author:	Mike Weiblen http://mew.cx/ 2005-04-22
 * copyright:	(C) 2005 Michael Weiblen
 * license:	OpenSceneGraph Public License (OSGPL)
*/

#include <osg/Notify>
#include <osg/Image>
#include <osg/ImageStream>

#include <osgDB/ReaderWriter>
#include <osgDB/FileNameUtils>
#include <osgDB/Registry>
#include <osgDB/ReadFile>

#define EXTENSION_NAME "movie"

///////////////////////////////////////////////////////////////////////////

/**
 * An OSG reader plugin for the ".movie" pseudo-loader, which loads an
 * image file and renders it as a movie.
 *
 * Usage: <file.ext>.movie
 *
 * example: osgviewer horsetooth.png.movie
 */

class ReaderWriterMOVIE : public osgDB::ReaderWriter
{
public:
    ReaderWriterMOVIE() { }
    
    virtual const char* className() const { return "movie pseudo-loader"; }

    virtual bool acceptsExtension(const std::string& extension) const
    { 
	return osgDB::equalCaseInsensitive( extension, EXTENSION_NAME );
    }

    virtual ReadResult readNode(const std::string& fileName, const osgDB::ReaderWriter::Options* options) const
    {
	std::string ext = osgDB::getLowerCaseFileExtension(fileName);
	if( !acceptsExtension(ext) )
	    return ReadResult::FILE_NOT_HANDLED;

	osg::notify(osg::INFO) << "ReaderWriterMOVIE( \"" << fileName << "\" )" << std::endl;

	// strip the pseudo-loader extension, which must leave a sub-filename.
	std::string subFileName = osgDB::getNameLessExtension( fileName );
	if( subFileName == fileName )
	{
	    osg::notify(osg::WARN) << "Missing subfilename for " EXTENSION_NAME " pseudo-loader" << std::endl;
	    return ReadResult::FILE_NOT_HANDLED;
	}

	// recursively load the subfile.
        osg::Image* image = readImageFile(subFileName.c_str(), options);
        if (!image)
	{
	    // propagate the read failure upwards
	    osg::notify(osg::WARN) << "Subfile \"" << subFileName << "\" could not be loaded" << std::endl;
	    return ReadResult::FILE_NOT_HANDLED;
	}

        osg::ref_ptr<osg::ImageStream> imageStream = dynamic_cast<osg::ImageStream*>(image);
	imageStream->play();
	return osg::createGeodeForImage(imageStream.get());
    }
};


// Add ourself to the Registry to instantiate the reader/writer.
osgDB::RegisterReaderWriterProxy<ReaderWriterMOVIE> g_readerWriter_MOVIE_Proxy;

/*EOF*/

