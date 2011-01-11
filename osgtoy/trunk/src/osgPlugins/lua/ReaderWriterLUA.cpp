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

/* file:      src/osgPlugins/lua/ReaderWriterLUA.cpp
 * author:    Mike Weiblen
 * copyright: (C) 2004-2006 Michael Weiblen http://mew.cx/
 * license:   OpenSceneGraph Public License (OSGPL)
 * $Id: ReaderWriterLUA.cpp,v 1.2 2006/07/15 23:57:43 mew Exp $
*/

#include <stdio.h>
#include <sstream>

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
}

#include <osg/Notify>
#include <osg/Node>
#include <osgDB/ReaderWriter>
#include <osgDB/FileNameUtils>
#include <osgDB/FileUtils>
#include <osgDB/Registry>
#include <osgDB/ReadFile>

#include <osgToy/LuaState.h>

#define EXTENSION_NAME "lua"

///////////////////////////////////////////////////////////////////////////

/**
 * An OSG reader plugin for lua scripts.
 */

class ReaderWriterLUA : public osgDB::ReaderWriter
{
public:
    ReaderWriterLUA()
    {
        supportsExtension( EXTENSION_NAME, "lua script loader" );
    }
    
    const char* className() const { return "lua script loader"; };

    ReadResult readNode( const std::string& fileStr, const osgDB::ReaderWriter::Options* options ) const
    {
        std::string ext( osgDB::getLowerCaseFileExtension(fileStr) );
        if( !acceptsExtension(ext) )
            return ReadResult::FILE_NOT_HANDLED;

        std::string fileName( osgDB::findDataFile( fileStr, options ) );
        if( fileName.empty() )
            return ReadResult::FILE_NOT_FOUND;

        osg::notify(osg::INFO)
            << "ReaderWriterLUA( \"" << fileName << "\" )" << std::endl;

        osgToy::LuaState L;
        if( luaL_loadfile( L, fileName.c_str()) || lua_pcall(L,0,1,0) )
        {
            osg::notify(osg::WARN) << "error in \"" << fileName << "\":\n" 
                    << lua_tostring(L,-1) << std::endl;
            return 0;
        }

        osg::Node* node(0);

        if( ! lua_islightuserdata(L,-1) )
        {
            osg::notify(osg::WARN) << "wrong lua return type \""
                    << lua_typename(L,lua_type(L,-1)) << "\"" << std::endl;
        }
        else
        {
            node = static_cast<osg::Node*>( lua_touserdata(L,-1) );
        }
        return node;
    }
};

// Add ourself to the Registry to instantiate the reader/writer.
osgDB::RegisterReaderWriterProxy<ReaderWriterLUA> g_readerWriter_LUA_Proxy;

// vim: set sw=4 ts=8 et ic ai:
