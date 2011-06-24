/* file:        src/osgToy/lualib_osg.cpp
 * author:      Mike Weiblen 2005-05-08
 * copyright:   (C) 2005-2006 Michael Weiblen
 * license:     OpenSceneGraph Public License (OSGPL)
 * website:     http://mew.cx/osg/
 * $Id$
*/

#include <osg/Node>
#include <osg/Geode>
#include <osgToy/LuaState.h>

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
}

///////////////////////////////////////////////////////////////////////////

int luaopen_osg( lua_State* L )
{
    // FUTURE
    return 0;
}

// vim: set sw=4 ts=8 et ic ai:
