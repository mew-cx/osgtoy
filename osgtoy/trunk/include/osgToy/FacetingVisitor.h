/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2004 Robert Osfield 
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

// $Id: FacetingVisitor,v 1.2 2006/09/11 06:24:04 mew Exp $

#ifndef OSGTOY_FACETINGVISITOR
#define OSGTOY_FACETINGVISITOR 1

#include <osg/NodeVisitor>
#include <osg/Geode>
#include <osg/Geometry>

#include <osgToy/Export.h>

namespace osgToy {

/** A faceting visitor for calculating face normals for osg::GeoSet's
  *  which contains surface primitives.
  * (Contrast w/ osgUtil::SmoothingVisitor)
  */
class OSGTOY_EXPORT FacetingVisitor : public osg::NodeVisitor
{
public:
    FacetingVisitor();
    virtual ~FacetingVisitor();
    
    /// create face normals for vertices
    static void facet(osg::Geometry& geoset);

    /// apply faceting method to all geode geosets.
    virtual void apply(osg::Geode& geode);
};

}

#endif

// vim: set sw=4 ts=8 et ic ai:
