/* file:        src/osgToy/LuaState.cpp
 * author:      Mike Weiblen 2005-05-08
 * copyright:   (C) 2005 Michael Weiblen
 * license:     OpenSceneGraph Public License (OSGPL)
 * website:     http://mew.cx/osg/
 * $Id: LuaState.cpp,v 1.1 2005/06/06 22:10:37 mew Exp $
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

    static const luaL_reg libs[] = {
        { "base",       luaopen_base },
        { "table",      luaopen_table },
        { "io",         luaopen_io },
        { "string",     luaopen_string },
        { "math",       luaopen_math },
        { "loadlib",    luaopen_loadlib },
        { "debug",      luaopen_debug },

        { "osg",        luaopen_osg },
        { "osgToy",     luaopen_osgToy },
        { 0, 0 }
    };

    for( const luaL_reg* lib = libs; lib->func; ++lib )
    {
        lib->func( _luaState );
        lua_settop( _luaState, 0 );     // discard any results
    }
}

osgToy::LuaState::~LuaState()
{
    lua_close( _luaState );
}

/* vim: set sw=4 ts=8 et ic ai: */
/*EOF*/
