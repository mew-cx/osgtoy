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

// mew 2004-12-21
// derived from osgUtil::SmoothingVisitor.cpp
// $Id: FacetingVisitor.cpp,v 1.2 2006/09/11 06:24:05 mew Exp $

#include <osg/TriangleIndexFunctor>

#include <osgToy/FacetingVisitor.h>

///////////////////////////////////////////////////////////////////////////

struct FacetingOperator
{
    FacetingOperator() : _vAry(0), _nAry(0) {}
    
    void set( osg::Vec3Array* vAry, osg::Vec3Array* nAry )
    {
        _vAry = vAry;
        _nAry = nAry;
    }

    inline void operator()(unsigned int p1, unsigned int p2, unsigned int p3)
    {
        osg::Vec3& v1 = (*_vAry)[p1];
        osg::Vec3& v2 = (*_vAry)[p2];
        osg::Vec3& v3 = (*_vAry)[p3];

        osg::Vec3 normal = (v2-v1)^(v3-v1);
        normal.normalize();

        (*_nAry)[p1] = (*_nAry)[p2] = (*_nAry)[p3] = normal;
    }

    osg::Vec3Array* _vAry;
    osg::Vec3Array* _nAry;
};

///////////////////////////////////////////////////////////////////////////

osgToy::FacetingVisitor::FacetingVisitor()
{
    setTraversalMode(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN);
}

osgToy::FacetingVisitor::~FacetingVisitor()
{
}

void osgToy::FacetingVisitor::facet( osg::Geometry& geom )
{
    // count #surfaces, exit if none
    osg::Geometry::PrimitiveSetList& primitives = geom.getPrimitiveSetList();
    osg::Geometry::PrimitiveSetList::iterator itr;
    unsigned int numSurfacePrimitives=0;
    for(itr=primitives.begin();
        itr!=primitives.end();
        ++itr)
    {
        switch((*itr)->getMode())
        {
            case osg::PrimitiveSet::TRIANGLES:
            case osg::PrimitiveSet::TRIANGLE_STRIP:
            case osg::PrimitiveSet::TRIANGLE_FAN:
            case osg::PrimitiveSet::QUADS:
            case osg::PrimitiveSet::QUAD_STRIP:
            case osg::PrimitiveSet::POLYGON:
                ++numSurfacePrimitives;
                break;

            default:
                break;
        }
    }
    if (!numSurfacePrimitives) return;

    // exit if no vertices
    osg::Vec3Array *coords = dynamic_cast<osg::Vec3Array*>(geom.getVertexArray());
    if (!coords || !coords->size()) return;
    
    // generate the normals
    osg::Vec3Array *normals = new osg::Vec3Array(coords->size());
    osg::TriangleIndexFunctor<FacetingOperator> ftif;
    ftif.set( coords, normals );
    geom.accept(ftif);

    geom.setNormalArray( normals );
    geom.setNormalIndices( geom.getVertexIndices() );
    geom.setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
    geom.dirtyDisplayList();
}


void osgToy::FacetingVisitor::apply(osg::Geode& geode)
{
    for(unsigned int i = 0; i < geode.getNumDrawables(); i++ )
    {
        osg::Geometry* geom = dynamic_cast<osg::Geometry*>(geode.getDrawable(i));
        if (geom) facet(*geom);
    }
}

// vim: set sw=4 ts=8 et ic ai:
