// $Id$
// $URL$
// Copyright 2004-2011 Mike Weiblen http://mew.cx/
// OpenSceneGraph Public License (OSGPL)

/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2004 Robert Osfield 
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * OpenSceneGraph Public License for more details.
*/

// The following symbol has a underscore suffix for compatibility.
// (with what? - mew)
#ifndef OSGTOY_EXPORT_
#define OSGTOY_EXPORT_ 1

#if defined(WIN32) && !(defined(__CYGWIN__) || defined(__MINGW32__))
    #pragma warning( disable : 4244 )
    #pragma warning( disable : 4251 )
    #pragma warning( disable : 4267 )
    #pragma warning( disable : 4275 )
    #pragma warning( disable : 4290 )
    #pragma warning( disable : 4786 )
    #pragma warning( disable : 4305 )
#endif

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__) || defined( __MWERKS__)
        #  ifdef OSGTOY_LIBRARY
        #    define OSGTOY_EXPORT   __declspec(dllexport)
        #  else
        #    define OSGTOY_EXPORT   __declspec(dllimport)
        #endif /* OSGTOY_LIBRARY */
#else
        #define OSGTOY_EXPORT 
#endif 

#endif

