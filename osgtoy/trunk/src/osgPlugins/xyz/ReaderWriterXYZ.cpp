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

/* file:      src/osgPlugins/xyz/ReaderWriterXYZ.cpp
 * author:    Mike Weiblen
 * copyright: (C) 2008 Michael Weiblen http://mew.cx/
 * license:   OpenSceneGraph Public License (OSGPL)
 * $Id: 2008-10-10 $
*/


// mew: quick hack loader to read XYZ space-delimited points
// from a text file.  
// Created to read Beverly Hill Hotel point cloud.


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

#define EXTENSION_NAME "xyz"

///////////////////////////////////////////////////////////////////////////

class ReaderWriterXYZ : public osgDB::ReaderWriter
{
public:
    ReaderWriterXYZ() {}
    ~ReaderWriterXYZ() {}
    
    const char* className() const { return "XYZ point cloud file loader"; }

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
    vAry->reserve( 40000000 );
    geom->setVertexArray( vAry );

    //osg::Vec3Array* nAry = new osg::Vec3Array( 40000000 );
    //geom->setNormalArray( nAry );
    //geom->setNormalBinding( osg::Geometry::BIND_PER_VERTEX );

    osg::Vec4Array* cAry = new osg::Vec4Array;
    geom->setColorArray( cAry );
    geom->setColorBinding( osg::Geometry::BIND_OVERALL );
    cAry->push_back( osg::Vec4(1,1,1,1) );

    while( is.good() )
    {
        float x, y, z;
        is >> x >> y >> z;

        x -= 35;
        y += 30;
        z += 20;

        vAry->push_back( osg::Vec3(x,y,z) );

        //osg::Vec3 normal(x,y,z);
        //normal.normalize();
        //nAry->push_back( normal );
    }

    geom->addPrimitiveSet( new osg::DrawArrays( GL_POINTS, 0, vAry->size() ) );

    //osg::StateSet* sset( geom->getOrCreateStateSet() );
    //sset->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
    return geom;
}


osgDB::ReaderWriter::ReadResult
ReaderWriterXYZ::readNode( const std::string& fileName,
                const osgDB::ReaderWriter::Options* options ) const
{
    std::string ext( osgDB::getLowerCaseFileExtension(fileName) );
    if( !acceptsExtension(ext) )
        return ReadResult::FILE_NOT_HANDLED;

    std::string fullFileName( osgDB::findDataFile( fileName, options ) );
    if( fullFileName.empty() )
        return ReadResult::FILE_NOT_FOUND;

    osg::notify(osg::INFO) << "ReaderWriterXYZ( \"" << fullFileName << "\" )" << std::endl;

    std::ifstream is;
    is.open( fullFileName.c_str(), std::ios_base::in );
    if( !is )
        return ReadResult::FILE_NOT_FOUND;

    osg::Geode* geode( new osg::Geode );
    geode->addDrawable( loadGeometry( is ) );
    is.close();
    return geode;
}

REGISTER_OSGPLUGIN( xyz, ReaderWriterXYZ )

// vim: set sw=4 ts=8 et ic ai:
