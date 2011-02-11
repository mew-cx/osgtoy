// $Id$
// $URL$
// Copyright 2005-2011 Mike Weiblen http://mew.cx/
// OpenSceneGraph Public License (OSGPL)
//
// see: http://lua.org/

#ifndef OSGTOY_LUASTATE
#define OSGTOY_LUASTATE 1

#include <osgToy/Export.h>

extern "C" struct lua_State;

namespace osgToy {

class OSGTOY_EXPORT LuaState
{
public:
    LuaState();
    virtual ~LuaState();
    inline operator lua_State* () {return _luaState;}

private:
    lua_State* _luaState;
};

}

#endif

// vim: set sw=4 ts=8 et ic ai:
