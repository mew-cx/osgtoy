/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2003 Robert Osfield 
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

#include <osgVRPN/Export>


extern "C" {

/**
 * osgVRPNGetVersion() returns the library version number.
 * Numbering convention : osg_src-0.8.31 will return 0.8.31.
 *
 * This C function can be also used to check for the existence of the OpenSceneGraph
 * library using autoconf and its m4 macro AC_CHECK_LIB.
 *
 * Here is the code to add to your configure.in:
 \verbatim
 #
 # Check for the OpenSceneGraph (OSG) VRPN library
 #
 AC_CHECK_LIB(osg, osgVRPNGetVersion, ,
    [AC_MSG_ERROR(OpenSceneGraph VRPN library not found. See http://www.openscenegraph.org)],)
 \endverbatim
*/
extern OSGVRPN_EXPORT const char* osgVRPNGetVersion();

/**
 * getLibraryName_osgVRPN() returns the library name in human friendly form.
*/
extern OSGVRPN_EXPORT const char* osgVRPNGetLibraryName();

}

#endif

