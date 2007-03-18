/* file:      src/osgPlugins/hpp/Image.h
 * author:    Mike Weiblen
 * copyright: (C) 2007 Michael Weiblen http://mew.cx/
 * license:   OpenSceneGraph Public License (OSGPL)
 * $Id: Image.h,v 1.3 2007/03/18 05:36:12 mew Exp $
*/

#ifndef OSGPLUGIN_HPP_IMAGE_H
#define OSGPLUGIN_HPP_IMAGE_H

#include <iostream>
#include <osg/Image>

namespace hpp
{

std::ostream& operator << (std::ostream& fout, const osg::Image& img );

}

#endif

// vim: set sw=4 ts=8 et ic ai:
