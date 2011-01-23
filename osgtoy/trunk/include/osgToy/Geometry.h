// file:        include/osgToy/Geometry.h
// author:      Mike Weiblen  http://mew.cx/  2011-01-11
// copyright:   (C) 2011 Mike Weiblen
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


class OSGTOY_EXPORT WireBox : public osgToy::Lines
{
public:
    WireBox( const osg::Vec3& vMin, const osg::Vec3& vMax ) { create( vMin, vMax ); }
    WireBox( const osg::BoundingBox& bb ) { create( bb._min, bb._max ); }

protected:
    void create( const osg::Vec3& min, const osg::Vec3& max );
    virtual ~WireBox() {}

private:
    WireBox();       // disallowed
};


class OSGTOY_EXPORT WireFrustum : public osgToy::Lines
{
public:
    WireFrustum( float fov, float aspect, float zNear, float zFar );

protected:
    virtual ~WireFrustum() {}

private:
    WireFrustum();       // disallowed
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
