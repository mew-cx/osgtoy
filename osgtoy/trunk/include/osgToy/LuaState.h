/* file:        include/osgToy/LuaState
 * author:      Mike Weiblen 2005-05-08
 * copyright:   (C) 2005-2006 Michael Weiblen
 * license:     OpenSceneGraph Public License (OSGPL)
 * website:     http://mew.cx/osg/
 * $Id: LuaState,v 1.2 2006/09/11 06:24:04 mew Exp $
 *
 * see: http://lua.org/
*/

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
