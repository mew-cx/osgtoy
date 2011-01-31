/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2005 Robert Osfield 
 * $Id$
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

#ifndef OSGVRPN_VERSION
#define OSGVRPN_VERSION 1

#include <osgVRPN/Export.h>

extern "C" {

extern OSGVRPN_EXPORT const char* osgVRPNGetVersion();
extern OSGVRPN_EXPORT const char* osgVRPNGetLibraryName();

extern OSGVRPN_EXPORT const char* osgVRPNSvnKeywordId();
extern OSGVRPN_EXPORT const char* osgVRPNSvnKeywordDate();
extern OSGVRPN_EXPORT const char* osgVRPNSvnKeywordAuthor();
extern OSGVRPN_EXPORT const char* osgVRPNSvnKeywordUrl();
extern OSGVRPN_EXPORT const char* osgVRPNSvnKeywordRevision();

}

#endif

// vim: set sw=4 ts=8 et ic ai:
