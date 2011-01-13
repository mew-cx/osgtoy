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

/* file:	src/osgPlugins/filelist/ReaderWriterFILELIST.cpp
 * author:	Mike Weiblen http://mew.cx/ 2011-01-10
 * copyright:	(C) 2011 Michael Weiblen
 * license:	OpenSceneGraph Public License (OSGPL)
*/

#include <osg/Notify>
#include <osg/ShapeDrawable>

#include <osgDB/ReaderWriter>
#include <osgDB/FileNameUtils>
#include <osgDB/Registry>
#include <osgDB/ReadFile>

#define EXTENSION_NAME "filelist"
// and "txt"

///////////////////////////////////////////////////////////////////////////

/**
 * An OSG reader plugin for the ".filelist" pseudo-loader, which reads a
 * list of filenames from a text file.  The files are loaded and added
 * as children to a osg::Group node.
 * Any portion of a line after a "#" commenting character is ignored.
 * Whitespace and blank lines are ignored.
 *
 * Usage: <file.ext>.filelist
 *
 * example: osgviewer myfiles.txt.filelist
 */

class ReaderWriterFILELIST : public osgDB::ReaderWriter
{
public:
    ReaderWriterFILELIST()
    {
        supportsExtension( EXTENSION_NAME, "filelist pseudo-loader" );
    }

    virtual const char* className() const { return "filelist pseudo-loader"; }

    virtual ReadResult readNode(const std::string& fileName, const osgDB::ReaderWriter::Options* options) const
    {
	std::string ext = osgDB::getLowerCaseFileExtension(fileName);
	if( !acceptsExtension(ext) )
	    return ReadResult::FILE_NOT_HANDLED;

	osg::notify(osg::INFO) << "ReaderWriterFILELIST( \"" << fileName << "\" )" << std::endl;






	// strip the pseudo-loader extension, which must leave a sub-filename.
	std::string subFileName = osgDB::getNameLessExtension( fileName );
	if( subFileName == fileName )
	{
	    osg::notify(osg::WARN) << "Missing subfilename for " EXTENSION_NAME " pseudo-loader" << std::endl;
	    return ReadResult::FILE_NOT_HANDLED;
	}






	// recursively load the subfile.
        osg::HeightField* hf = readHeightFieldFile(subFileName.c_str(), options);
        if (!hf)
	{
	    // propagate the read failure upwards
	    osg::notify(osg::WARN) << "Subfile \"" << subFileName << "\" could not be loaded" << std::endl;
	    return ReadResult::FILE_NOT_HANDLED;
	}






	osg::Geode* geode = new osg::Geode;
	geode->addDrawable(new osg::ShapeDrawable(hf));
	return geode;
    }
};


// Add ourself to the Registry to instantiate the reader/writer.
osgDB::RegisterReaderWriterProxy<ReaderWriterFILELIST> g_readerWriter_FILELIST_Proxy;

/*EOF*/
