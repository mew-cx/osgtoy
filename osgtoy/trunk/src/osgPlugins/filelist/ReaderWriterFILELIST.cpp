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

/* $Id$
 * $URL$
 * author:	Mike Weiblen http://mew.cx/ 2011-04-04
 * copyright:	(C) 2011 Michael Weiblen
 * license:	OpenSceneGraph Public License (OSGPL)
*/

#include <osg/Notify>
#include <osg/ShapeDrawable>

#include <osgDB/ReaderWriter>
#include <osgDB/FileNameUtils>
#include <osgDB/Registry>
#include <osgDB/ReadFile>

#include <string>
#include <iostream>
#include <sstream>
#include <osgToy/TextFileReader.h>

#define EXTENSION_NAME "filelist"	// and "txt"?


///////////////////////////////////////////////////////////////////////////

namespace {

class FilelistReader : public osgToy::TextFileReader
{
public:
    FilelistReader( osg::Group* group ) : _group(group) {}
    virtual ~FilelistReader() {}

    osg::Group* getGroup() const { return _group; }

protected:        // methods
    virtual void evaluate( std::string& filename );

private:
    FilelistReader();   // disallowed
    osg::Group* _group;

};


void FilelistReader::evaluate( std::string& filename )
{
    if( filename.empty() )
        return;

    osg::notify(osg::INFO) << "reading file \"" << filename << "\"" << std::endl;
    osg::Node* node( osgDB::readNodeFile( filename /*, options */ ) );
    if( node )
    {
        _group->addChild( node );
    }
    else
    {
        osg::notify(osg::WARN) << "file \"" << filename << "\" could not be loaded" << std::endl;
    }
}

}


/////////////////////////////////////////////////////////////////////////////

class ReaderWriterFILELIST : public osgDB::ReaderWriter
{
public:
    ReaderWriterFILELIST()
    {
        supportsExtension( EXTENSION_NAME, "filelist pseudo-loader" );
	// TODO:
	// add "prefix" option
	// add "suffix" option, ex ".bbox"
    }

    virtual const char* className() const { return "filelist pseudo-loader"; }

    virtual ReadResult readNode( const std::string& fileName, const osgDB::ReaderWriter::Options* options ) const
    {
	std::string ext = osgDB::getLowerCaseFileExtension(fileName);
	if( !acceptsExtension(ext) )
	    return ReadResult::FILE_NOT_HANDLED;

	osg::notify(osg::INFO) << "ReaderWriterFILELIST( \"" << fileName << "\" )" << std::endl;

        FilelistReader flr( new osg::Group );
        if( ! flr.readFile( fileName ) )
	{
	    // propagate the read failure upwards
	    osg::notify(osg::WARN) << "filelist \"" << fileName << "\" could not be loaded" << std::endl;
	    return ReadResult::FILE_NOT_HANDLED;
	}
	return flr.getGroup();
    }
};

// Add ourself to the Registry to instantiate the reader/writer.
osgDB::RegisterReaderWriterProxy<ReaderWriterFILELIST> g_readerWriter_FILELIST_Proxy;

// vim: set sw=4 ts=8 et ic ai:
