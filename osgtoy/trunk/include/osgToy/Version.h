/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2005 Robert Osfield 
 * $Id: SvnKeywords.h 573 2011-01-29 17:11:21Z mew $
 *
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

#ifndef OSGTOY_VERSION
#define OSGTOY_VERSION 1

#include <osgToy/Export.h>

extern "C" {

extern OSGTOY_EXPORT const char* osgToyGetVersion();
extern OSGTOY_EXPORT const char* osgToyGetLibraryName();

extern OSGTOY_EXPORT const char* osgToySvnKeywordId();
extern OSGTOY_EXPORT const char* osgToySvnKeywordDate();
extern OSGTOY_EXPORT const char* osgToySvnKeywordAuthor();
extern OSGTOY_EXPORT const char* osgToySvnKeywordUrl();
extern OSGTOY_EXPORT const char* osgToySvnKeywordRevision();

}

#endif

// vim: set sw=4 ts=8 et ic ai:
