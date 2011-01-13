// file:        include/osgToy/Geometry.h
// author:      Mike Weiblen  http://mew.cx/  2008-11-06
// copyright:   (C) 2008 Mike Weiblen
// license:     OpenSceneGraph Public License (OSGPL)


#ifndef OSGTOY_GEOMETRY
#define OSGTOY_GEOMETRY

#include <osg/Geometry>
#include <osg/BoundingBox>
#include <osgToy/Export.h>


namespace osgToy {

class OSGTOY_EXPORT Geometry : public osg::Geometry
{
public:
    Geometry( unsigned int numVerts = 0 );

    void setOverallColor( const osg::Vec4& color );

protected:
    virtual ~Geometry() {}
};


class OSGTOY_EXPORT Quad : public Geometry
{
public:
    Quad( float width, float height, int texUnit = -1 );

protected:
    virtual ~Quad() {}

private:
    Quad();             // disallowed
};


class OSGTOY_EXPORT Lines : public Geometry
{
public:
    Lines();
    void addLine( const osg::Vec3& a, const osg::Vec3& b );

protected:
    virtual ~Lines() {}
};


// http://en.wikipedia.org/wiki/Prismatoid
class OSGTOY_EXPORT WirePrismatoid : public osgToy::Lines
{
public:
    WirePrismatoid( const osg::Vec3& vMin, const osg::Vec3& vMax ) { create( vMin, vMax ); }
    WirePrismatoid( const osg::BoundingBox& bb ) { create( bb._min, bb._max ); }
    WirePrismatoid( float fov, float aspect, float zNear, float zFar );

protected:
    virtual ~WirePrismatoid() {}
    void create( const osg::Vec3& nnn, const osg::Vec3& xxx );

private:
    WirePrismatoid();          // disallowed
};


/////////////////////////////////////////////////////////////////////////////

class OSGTOY_EXPORT Grid : public osgToy::Lines         // TODO
{
public:
    Grid( const osg::Vec3& position, const osg::Vec3& normal );
protected:
    virtual ~Grid() {}
private:
    Grid();             // disallowed
};


class OSGTOY_EXPORT Vec3 : public Geometry    // TODO
{
public:
    Vec3( const osg::Vec3& v );
    // color, offset, scale , etc
protected:
    virtual ~Vec3() {}
private:
    Vec3();             // disallowed
};

}

#endif

// vim: set sw=4 ts=8 et ic ai:
