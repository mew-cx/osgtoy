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

/* file:	src/osgPlugins/simplifier/ReaderWriterSIMPLIFIER.cpp
 * author:	Mike Weiblen http://mew.cx/ 2004-11-30
 * copyright:	(C) 2004 Michael Weiblen
 * license:	OpenSceneGraph Public License (OSGPL)
*/

#include <osg/Notify>

#include <osgDB/ReaderWriter>
#include <osgDB/FileNameUtils>
#include <osgDB/Registry>
#include <osgDB/ReadFile>

#include <osgUtil/Simplifier>

#include <stdio.h>

#define EXTENSION_NAME "simplifier"

///////////////////////////////////////////////////////////////////////////

/**
 * An OSG reader plugin for the ".simplifier" pseudo-loader, which runs
 * osgUtil::Simplifier on the loaded model.
 * This pseudo-loader makes it simple to simplify a saved model by
 * specifying a correcting sampling percentage as part of the filename.
 *
 * Usage: <modelfile.ext>.<sp>.simplifier
 * where:
 *	<modelfile.ext> = an model filename.
 *	<sp> = sampling percentage (integer, 1 to 100)
 *
 * example: osgviewer cow.osg.20.simplifier
 */

class ReaderWriterSIMPLIFIER : public osgDB::ReaderWriter
{
public:
    ReaderWriterSIMPLIFIER() { }
    
    virtual const char* className() const { return "osgUtil::Simplifier pseudo-loader"; }

    virtual bool acceptsExtension(const std::string& extension) const
    { 
	return osgDB::equalCaseInsensitive( extension, EXTENSION_NAME );
    }

    virtual ReadResult readNode(const std::string& fileName, const osgDB::ReaderWriter::Options* options) const
    {
	std::string ext = osgDB::getLowerCaseFileExtension(fileName);
	if( !acceptsExtension(ext) )
	    return ReadResult::FILE_NOT_HANDLED;

	osg::notify(osg::INFO) << "ReaderWriterSIMPLIFIER( \"" << fileName << "\" )" << std::endl;

	// strip the pseudo-loader extension
	std::string tmpName = osgDB::getNameLessExtension( fileName );

	// get the next "extension", which actually contains the pseudo-loader parameters
	std::string params = osgDB::getFileExtension( tmpName );
	if( params.empty() )
	{
	    osg::notify(osg::WARN) << "Missing parameters for " EXTENSION_NAME " pseudo-loader" << std::endl;
	    return ReadResult::FILE_NOT_HANDLED;
	}

	// strip the "params extension", which must leave a sub-filename.
	std::string subFileName = osgDB::getNameLessExtension( tmpName );
	if( subFileName == tmpName )
	{
	    osg::notify(osg::WARN) << "Missing subfilename for " EXTENSION_NAME " pseudo-loader" << std::endl;
	    return ReadResult::FILE_NOT_HANDLED;
	}

	osg::notify(osg::INFO) << EXTENSION_NAME " params = \"" << params << "\"" << std::endl;

	int samplePercentage;
	int count = sscanf( params.c_str(), "%d", &samplePercentage );
	if( (count != 1) || (samplePercentage < 1) || (samplePercentage > 100) )
	{
	    osg::notify(osg::WARN) << "Bad parameters for " EXTENSION_NAME " pseudo-loader: \"" << params << "\"" << std::endl;
	    return ReadResult::FILE_NOT_HANDLED;
	}

	// recursively load the subfile.
	osg::Node *node = osgDB::readNodeFile( subFileName, options );
	if( !node )
	{
	    // propagate the read failure upwards
	    osg::notify(osg::WARN) << "Subfile \"" << subFileName << "\" could not be loaded" << std::endl;
	    return ReadResult::FILE_NOT_HANDLED;
	}

	float sampleRatio = samplePercentage / 100.0;
	osgUtil::Simplifier simplifier( sampleRatio );
	node->accept( simplifier );
	return node;
    }
};


// Add ourself to the Registry to instantiate the reader/writer.
osgDB::RegisterReaderWriterProxy<ReaderWriterSIMPLIFIER> g_readerWriter_SIMPLIFIER_Proxy;

/*EOF*/
