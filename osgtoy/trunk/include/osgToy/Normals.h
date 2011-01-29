// This code originally developed and posted to the osg-users mailing list
// by Don Burns on 2004-12-10; see:
// http://openscenegraph.org/pipermail/osg-users/2004-December/048243.html
 * $Id$

#ifndef OSGTOY_NORMALS_H
#define OSGTOY_NORMALS_H

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/NodeVisitor>

#include <osgToy/Export.h>

namespace osgToy {

class OSGTOY_EXPORT Normals : public osg::Geode 
{
public:
    enum Mode {
        SurfaceNormals,
        VertexNormals
    };

    Normals( osg::Node *node, float scale=1.0, Mode mode=SurfaceNormals );

protected:
    ~Normals() {}

private:

    class MakeNormalsVisitor : public osg::NodeVisitor
    {
    public:
        MakeNormalsVisitor(float normalScale=1.0, Normals::Mode=Normals::SurfaceNormals );

        void setMode( Mode mode ) { _mode = mode; }

        void apply( osg::Geode &geode );

        osg::Vec3Array *getCoords() { return _local_coords.get(); }

    private:
        osg::ref_ptr<osg::Vec3Array> _local_coords;
        float _normal_scale;
        Mode _mode;

        void _processPrimitive(  unsigned int nv,
                osg::Vec3Array::iterator coords, 
                osg::Vec3Array::iterator normals,
                osg::Geometry::AttributeBinding binding );
    };

    // For debugging
    static void _printPrimitiveType( osg::PrimitiveSet *pset );
};


class OSGTOY_EXPORT SurfaceNormals : public Normals
{
public:
    SurfaceNormals( Node *node, float scale=1.0 ):
            Normals( node, scale, Normals::SurfaceNormals ) {}
protected:
    ~SurfaceNormals() {}
};


class OSGTOY_EXPORT VertexNormals : public Normals
{
public:
    VertexNormals( Node *node, float scale=1.0 ):
            Normals( node, scale, Normals::VertexNormals ) {}
protected:
    ~VertexNormals() {}
};

}

#endif

// vim: set sw=4 ts=8 et ic ai:
