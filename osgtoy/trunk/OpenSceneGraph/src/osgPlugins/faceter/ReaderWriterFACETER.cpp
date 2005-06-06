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

/* file:	src/osgPlugins/faceter/ReaderWriterFACETER.cpp
 * author:	Mike Weiblen http://mew.cx/ 2004-12-22
 * copyright:	(C) 2004 Michael Weiblen
 * license:	OpenSceneGraph Public License (OSGPL)
*/

#include <osg/Notify>
#include <osg/Node>
#include <osg/StateSet>

#include <osgDB/ReaderWriter>
#include <osgDB/FileNameUtils>
#include <osgDB/Registry>
#include <osgDB/ReadFile>

#include <osgToy/FacetingVisitor>

#define EXTENSION_NAME "faceter"

///////////////////////////////////////////////////////////////////////////

/**
 * An OSG reader plugin for the ".faceter" pseudo-loader, which runs
 * osgToy::FacetingVisitor on the loaded model.
 *
 * Usage: <modelfile.ext>.faceter
 * where:
 *	<modelfile.ext> = an model filename.
 *
 * example: osgviewer vase.osg.faceter
 */

class ReaderWriterFACETER : public osgDB::ReaderWriter
{
public:
    ReaderWriterFACETER() { }
    
    virtual const char* className() const { return "FacetingVisitor pseudo-loader"; }

    virtual bool acceptsExtension(const std::string& extension) const
    { 
	return osgDB::equalCaseInsensitive( extension, EXTENSION_NAME );
    }

    virtual ReadResult readNode(const std::string& fileName,
		const osgDB::ReaderWriter::Options* /*options*/) const
    {
	std::string ext = osgDB::getLowerCaseFileExtension(fileName);
	if( !acceptsExtension(ext) )
	    return ReadResult::FILE_NOT_HANDLED;

	osg::notify(osg::INFO) << "ReaderWriterFACETER( \"" << fileName << "\" )" << std::endl;

	// strip the pseudo-loader extension
	std::string subFileName = osgDB::getNameLessExtension( fileName );
	if( subFileName == fileName )
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

	osgToy::FacetingVisitor fv;
	node->accept( fv );

	osg::StateSet* sset = node->getOrCreateStateSet();
	sset->setMode( GL_LIGHTING, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE );

	return node;
    }
};


// Add ourself to the Registry to instantiate the reader/writer.
osgDB::RegisterReaderWriterProxy<ReaderWriterFACETER> g_readerWriter_FACETER_Proxy;

/*EOF*/
