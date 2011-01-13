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

/* file:      src/osgPlugins/csv/ReaderWriterCSV.cpp
 * author:    Mike Weiblen
 * copyright: (C) 2008 Michael Weiblen http://mew.cx/
 * license:   OpenSceneGraph Public License (OSGPL)
 * $Id: 2008-10-10 $
*/

// mew: quick hack to read XYZI comma-delimited vec4s from text file.
// created to read Radiohead House of Cards point cloud.


#include <stdio.h>
#include <iostream>
#include <sstream>

#include <osg/Notify>
#include <osg/Node>
#include <osg/Geode>
#include <osg/Geometry>
#include <osgDB/ReaderWriter>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>
#include <osgDB/Registry>
#include <osgDB/ReadFile>

#define EXTENSION_NAME "csv"

///////////////////////////////////////////////////////////////////////////

class ReaderWriterCSV : public osgDB::ReaderWriter
{
public:
    ReaderWriterCSV() {}
    ~ReaderWriterCSV() {}
    
    const char* className() const { return "CSV (comma separated value) file loader"; }

    bool acceptsExtension(const std::string& extension) const
    {
        return osgDB::equalCaseInsensitive( extension, EXTENSION_NAME );
    }

    ReadResult readNode( const std::string&, const osgDB::ReaderWriter::Options* ) const;
};

///////////////////////////////////////////////////////////////////////////

osg::Geometry* loadGeometry( std::istream& is )
{
    osg::Geometry* geom( new osg::Geometry );

    osg::Vec3Array* vAry = new osg::Vec3Array;
    geom->setVertexArray( vAry );

    osg::Vec4Array* cAry = new osg::Vec4Array;
    geom->setColorArray( cAry );
    geom->setColorBinding( osg::Geometry::BIND_PER_VERTEX );

    while( is.good() )
    {
        std::string line;
        std::getline( is, line, '\n' );
        if( !line.empty() )
        {
            std::istringstream ss( line );
            char c(0);
            float x, y, z, i;
            ss >> x >> c >> y >> c >> z >> c >> i;
            i /= 255.0f;

            vAry->push_back( osg::Vec3(x,y,z) );
            cAry->push_back( osg::Vec4(i,i,i,1) );
        }
    }

    geom->addPrimitiveSet( new osg::DrawArrays( GL_POINTS, 0, vAry->size() ) );

    osg::StateSet* sset( geom->getOrCreateStateSet() );
    sset->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
    return geom;
}


osgDB::ReaderWriter::ReadResult
ReaderWriterCSV::readNode( const std::string& fileName,
                const osgDB::ReaderWriter::Options* options ) const
{
    std::string ext( osgDB::getLowerCaseFileExtension(fileName) );
    if( !acceptsExtension(ext) )
        return ReadResult::FILE_NOT_HANDLED;

    std::string fullFileName( osgDB::findDataFile( fileName, options ) );
    if( fullFileName.empty() )
        return ReadResult::FILE_NOT_FOUND;

    osg::notify(osg::INFO) << "ReaderWriterCSV( \"" << fullFileName << "\" )" << std::endl;

    std::ifstream is;
    is.open( fullFileName.c_str(), std::ios_base::in );
    if( !is )
        return ReadResult::FILE_NOT_FOUND;

    osg::Geode* geode( new osg::Geode );
    geode->addDrawable( loadGeometry( is ) );
    is.close();
    return geode;
}

REGISTER_OSGPLUGIN( csv, ReaderWriterCSV )

// vim: set sw=4 ts=8 et ic ai:
