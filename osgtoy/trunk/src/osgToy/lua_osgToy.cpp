/* file:        src/osgToy/lua_osgToy.cpp
 * author:      Mike Weiblen 2005-05-08
 * copyright:   (C) 2005-2006 Michael Weiblen
 * license:     OpenSceneGraph Public License (OSGPL)
 * website:     http://mew.cx/osg/
 * $Id$
*/

#include <osg/Node>
#include <osg/Geode>
#include <osgToy/LuaState.h>
#include <osgToy/SuperShape3D.h>
#include <osgToy/LorenzAttractor.h>

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
}

///////////////////////////////////////////////////////////////////////////

static lua_Number getnum( lua_State* L, const char* key )
{
    lua_pushstring( L, key );
    lua_gettable( L, -2 );
    lua_Number val = luaL_checknumber( L, -1 );
    lua_pop( L, 1 );
    return val;
}

#define GETNUM(var,type) var = ((type)(getnum(L,#var)))

///////////////////////////////////////////////////////////////////////////
// Registered functions must conform to the lua_CFunction type in lua.h
// Returned integer indicates the #items being returned at top of stack.

static int l_SuperShape3D( lua_State* L )
{
    if( (lua_gettop(L) != 1) || !lua_istable(L,1) )
        luaL_error( L, "requires a table as the single argument" );

    float ss1_m, ss1_a, ss1_b, ss1_n1, ss1_n2, ss1_n3;
    float ss2_m, ss2_a, ss2_b, ss2_n1, ss2_n2, ss2_n3;
    int resolution;

    GETNUM( ss1_m, float );
    GETNUM( ss1_a, float );
    GETNUM( ss1_b, float );
    GETNUM( ss1_n1, float );
    GETNUM( ss1_n2, float );
    GETNUM( ss1_n3, float );
    GETNUM( ss2_m, float );
    GETNUM( ss2_a, float );
    GETNUM( ss2_b, float );
    GETNUM( ss2_n1, float );
    GETNUM( ss2_n2, float );
    GETNUM( ss2_n3, float );
    GETNUM( resolution, int );

    lua_pushstring( L, "name" );
    lua_gettable( L, -2 );
    const char* name = luaL_optstring( L, -1, 0 );
    lua_pop( L, 1 );

    osg::Geode* geode = new osg::Geode();
    if( name ) geode->setName( name );
    geode->addDrawable( new osgToy::SuperShape3D(
            ss1_m, ss1_a, ss1_b, ss1_n1, ss1_n2, ss1_n3,
            ss2_m, ss2_a, ss2_b, ss2_n1, ss2_n2, ss2_n3,
            resolution ) );

    lua_pushlightuserdata( L, geode );
    return 1;
}


static int l_LorenzAttractor( lua_State* L )
{
    if( (lua_gettop(L) != 1) || !lua_istable(L,1) )
        luaL_error( L, "requires a table as the single argument" );

    int numPoints;
    float deltaT, t0, a, b, c;
    osg::Vec3 p0 = osg::Vec3(1,0,0);

    GETNUM( numPoints, int );
    GETNUM( deltaT, float );
    //getVec3( L, "p0", p0 );   TODO Vec3 = table of 3 floats?
    GETNUM( t0, float );
    GETNUM( a, float );
    GETNUM( b, float );
    GETNUM( c, float );

    lua_pushstring( L, "name" );
    lua_gettable( L, -2 );
    const char* name = luaL_optstring( L, -1, 0 );
    lua_pop( L, 1 );

    osg::Geode* geode = new osg::Geode();
    if( name ) geode->setName( name );
    geode->addDrawable( new osgToy::LorenzAttractor(numPoints, deltaT, p0, t0, a, b, c) );

    lua_pushlightuserdata( L, geode );
    return 1;
}

///////////////////////////////////////////////////////////////////////////

int luaopen_osgToy( lua_State* L )
{
    static const luaL_reg funcs[] = {
        { "SuperShape3D", l_SuperShape3D },
        { "LorenzAttractor", l_LorenzAttractor },
        { 0, 0 }
    };

    luaL_openlib( L, "osgToy", funcs, 0 );
    return 1;     // the library's table on the stack
}

// vim: set sw=4 ts=8 et ic ai:
