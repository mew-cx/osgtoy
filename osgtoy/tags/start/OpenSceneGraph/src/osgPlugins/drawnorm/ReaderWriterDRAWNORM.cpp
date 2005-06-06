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

/* file:	src/osgPlugins/drawnorm/ReaderWriterDRAWNORM.cpp
 * author:	Mike Weiblen http://mew.cx/ 2004-12-22
 * copyright:	(C) 2004 Michael Weiblen
 * license:	OpenSceneGraph Public License (OSGPL)
*/

#include <stdio.h>

#include <osg/Notify>
#include <osg/Group>
#include <osg/ref_ptr>

#include <osgDB/ReaderWriter>
#include <osgDB/FileNameUtils>
#include <osgDB/Registry>
#include <osgDB/ReadFile>

#include <osgToy/Normals>

#define EXTENSION_NAME "drawnorm"

///////////////////////////////////////////////////////////////////////////

/**
 * An OSG reader plugin for the drawnorm pseudo-loader.
 */
class ReaderWriterDRAWNORM : public osgDB::ReaderWriter
{
public:
    ReaderWriterDRAWNORM() { }
    
    virtual const char* className() const { return "drawnorm pseudo-loader"; }

    virtual bool acceptsExtension(const std::string& extension) const
    { 
	return osgDB::equalCaseInsensitive( extension, EXTENSION_NAME );
    }

    virtual ReadResult readNode(const std::string& fileName,
		const osgDB::ReaderWriter::Options* options) const
    {
	std::string ext = osgDB::getLowerCaseFileExtension(fileName);
	if( !acceptsExtension(ext) )
	    return ReadResult::FILE_NOT_HANDLED;

	osg::notify(osg::INFO) << "ReaderWriterDRAWNORM( \"" << fileName << "\" )" << std::endl;

	// strip the pseudo-loader extension
	std::string subFileName = osgDB::getNameLessExtension( fileName );

	// recursively load the subfile.
	osg::Node *node = osgDB::readNodeFile( subFileName, options );
	if( !node )
	{
	    // propagate the read failure upwards
	    osg::notify(osg::WARN) << "Subfile \"" << subFileName << "\" could not be loaded" << std::endl;
	    return ReadResult::FILE_NOT_HANDLED;
	}

	osg::Group* group = new osg::Group;
	group->addChild( node );

	//osgToy::SurfaceNormals* sn = new osgToy::SurfaceNormals( node );
	//group->addChild( sn );

	osgToy::VertexNormals*  vn = new osgToy::VertexNormals( node );
	group->addChild( vn );

	return group;
    }
};


// Add ourself to the Registry to instantiate the reader/writer.
osgDB::RegisterReaderWriterProxy<ReaderWriterDRAWNORM> g_readerWriter_DRAWNORM_Proxy;

/* vim: set sw=4 ts=8 ic ai: */
/*EOF*/
