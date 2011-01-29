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

/* file:      src/osgPlugins/wirebox/ReaderWriterWIREBOX.cpp
 * author:    Mike Weiblen http://mew.cx/ 2011-01-11
 * copyright: (C) 2011 Michael Weiblen
 * license:   OpenSceneGraph Public License (OSGPL)
*/

#include <osg/Geode>
#include <osg/Notify>

#include <osgDB/ReaderWriter>
#include <osgDB/FileNameUtils>
#include <osgDB/Registry>
#include <osgDB/ReadFile>

#include <osgToy/Geometry.h>

#include <stdio.h>

#define EXTENSION_NAME "wirebox"


static bool splitFilename( std::string& params, std::string& subfile )
{
    unsigned int nesting(0);
    for( int pos = params.size()-1; pos >= 0; --pos )
    {
        switch( params[pos] )
        {
            case ']':
            case ')':
                params.erase( pos, 1 );
                ++nesting;
                break;

            case '[':
            case '(':
                params.erase( pos, 1 );
                --nesting;
                break;

            case '.':
                if( nesting == 0 )
                {
                    if( pos > 0 )
                        subfile = params.substr( 0, pos-1 );
                    params = params.substr( pos+1 );
                    return true;
                }
                break;
        }
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////

/**
 * An OSG reader plugin for the ".wirebox" pseudo-loader.
 *
 * Usage:
 *      <s>.wirebox = create centered cube.
 *      <x>,<y>,<z>.wirebox = create centered box.
 *      <fovy>,<aspect>,<znear>,<zfar>.wirebox = create frustum.
 *      <x1>,<y1>,<z1>,<x2>,<y2>,<z2>.wirebox = create box.
 */

class ReaderWriterWIREBOX : public osgDB::ReaderWriter
{
public:
    ReaderWriterWIREBOX() { }
    
    virtual const char* className() const { return EXTENSION_NAME " pseudo-loader"; }

    virtual bool acceptsExtension( const std::string& extension ) const
    { 
        return osgDB::equalCaseInsensitive( extension, EXTENSION_NAME );
    }

    void usage() const
    {
        osg::notify(osg::NOTICE)
            << "\nUsage for " << className() << ":\n"
            << "<s>." EXTENSION_NAME "\n\tcreate centered cube.\n"
            << "<x>,<y>,<z>." EXTENSION_NAME "\n\tcreate centered box.\n"
            << "<fovy>,<aspect>,<znear>,<zfar>." EXTENSION_NAME "\n\tcreate frustum.\n"
            << "<x1>,<y1>,<z1>,<x2>,<y2>,<z2>." EXTENSION_NAME "\n\tcreate box.\n"
            << std::endl;
    }

    virtual ReadResult readNode( const std::string& fileName, const osgDB::ReaderWriter::Options* options ) const
    {
        std::string ext( osgDB::getLowerCaseFileExtension(fileName) );
        if( ! acceptsExtension(ext) )
            return ReadResult::FILE_NOT_HANDLED;

        osg::notify(osg::INFO) << "ReaderWriterWIREBOX( \"" << fileName << "\" )" << std::endl;

        // remove the pseudo-loader extension
        std::string params( osgDB::getNameLessExtension( fileName ) );
        if( params.empty() )
            return ReadResult::FILE_NOT_HANDLED;

        std::string subfile;
        splitFilename( params, subfile );
        if( params.empty() )
        {
            osg::notify(osg::WARN) << "Missing parameters for " << className() << std::endl;
            return ReadResult::FILE_NOT_HANDLED;
        }

        osg::notify(osg::INFO) << "params = \"" << params << "\"" << std::endl;

        osg::Drawable* drawable(0);

        float x1, y1, z1, x2, y2, z2;
        const int count( sscanf( params.c_str(), "%f,%f,%f,%f,%f,%f", &x1, &y1, &z1, &x2, &y2, &z2 ) );

        switch( count )
        {
            case 1:
            {
                // axis-aligned cube centered at origin
                const float s(x1/2);
                osg::Vec3 vMax( s, s, s );
                osg::Vec3 vMin( -vMax );
                drawable = new osgToy::WireBox( vMin, vMax );
                break;
            }

            case 3:
            {
                // axis-aligned box centered at origin
                osg::Vec3 vMax( x1/2, y1/2, z1/2 );
                osg::Vec3 vMin( -vMax );
                drawable = new osgToy::WireBox( vMin, vMax );
                break;
            }

            case 4:
            {
                // frustum( fovy, aspect, znear, zfar )
                drawable = new osgToy::WireFrustum( x1, y1, z1, x2 );
                break;
            }

            case 6:
            {
                // axis-aligned box defined by min/max corners
                osg::Vec3 vMin( x1, y1, z1 );
                osg::Vec3 vMax( x2, y2, z2 );
                drawable = new osgToy::WireBox( vMin, vMax );
                break;
            }

            default:
                osg::notify(osg::WARN)
                    << "Bad parameters for " << className()
                    << ": \"" << params << "\"" << std::endl;
                usage();
                return ReadResult::FILE_NOT_HANDLED;
        }

        osg::Geode* geode( new osg::Geode );
        geode->addDrawable( drawable );
        return geode;
    }
};


// Add ourself to the Registry to instantiate the reader/writer.
REGISTER_OSGPLUGIN( wirebox, ReaderWriterWIREBOX )

// vim: set sw=4 ts=8 et ic ai:
