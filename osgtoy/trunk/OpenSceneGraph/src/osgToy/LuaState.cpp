/* file:        src/osgToy/LuaState.cpp
 * author:      Mike Weiblen 2005-05-08
 * copyright:   (C) 2005-2006 Michael Weiblen
 * license:     OpenSceneGraph Public License (OSGPL)
 * website:     http://mew.cx/osg/
 * $Id: LuaState.cpp,v 1.3 2006/09/11 06:24:05 mew Exp $
*/

#include <osgToy/LuaState>

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

int luaopen_osg( lua_State* L );
int luaopen_osgToy( lua_State* L );

///////////////////////////////////////////////////////////////////////////

osgToy::LuaState::LuaState() :
    _luaState( lua_open() )
{
    if( !_luaState ) throw "lua_open() failed";

    luaL_openlibs( _luaState );

    static const luaL_Reg libs[] = {
        { "osg",        luaopen_osg },
        { "osgToy",     luaopen_osgToy },
        { 0, 0 }
    };

    for( const luaL_Reg* lib = libs; lib->func; ++lib )
    {
        lua_pushcfunction( _luaState, lib->func );
        lua_pushstring( _luaState, lib->name );
        lua_call( _luaState, 1, 0 );
    }
}

osgToy::LuaState::~LuaState()
{
    lua_close( _luaState );
}

// vim: set sw=4 ts=8 et ic ai:
