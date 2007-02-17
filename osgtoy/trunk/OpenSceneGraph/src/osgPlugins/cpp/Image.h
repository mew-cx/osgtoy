/* file:      src/osgPlugins/cpp/Image.cpp
 * author:    Mike Weiblen http://mew.cx/
 * copyright: (C) 2007 Michael Weiblen
 * license:   OpenSceneGraph Public License (OSGPL)
 * $Id: Image.h,v 1.2 2007/02/17 04:41:44 mew Exp $
*/

#ifndef OSGPLUGIN_CPP_IMAGE_H
#define OSGPLUGIN_CPP_IMAGE_H

#include <iostream>
#include <osg/Image>

namespace cpp
{

std::ostream& operator << (std::ostream& fout, const osg::Image& img );

}

#endif

// vim: set sw=4 ts=8 et ic ai:
