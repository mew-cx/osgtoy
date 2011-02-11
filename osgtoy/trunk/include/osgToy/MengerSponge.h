// $Id$
// $URL$
// Copyright 2004-2011 Mike Weiblen http://mew.cx/
// OpenSceneGraph Public License (OSGPL)
//
// see: http://en.wikipedia.org/wiki/Menger_sponge
//      http://mathworld.wolfram.com/MengerSponge.html

#ifndef OSGTOY_MENGERSPONGE
#define OSGTOY_MENGERSPONGE 1

#include <osg/Geometry>

#include <osgToy/Export.h>

namespace osgToy {

/** The Menger sponge is a fractal solid of dimension ln20/ln3 (~2.727) */

class OSGTOY_EXPORT MengerSponge : public osg::Geometry
{
public:
    MengerSponge( unsigned int level = 1, float scale = 1.0f );

    unsigned int getNumQuads() const { return _numQuads; }

protected:
    virtual ~MengerSponge() {}

private:
    void addQuad( const osg::Vec3&, const osg::Vec3&, const osg::Vec3&, const osg::Vec3& );
    void subdivide( int, float, float, float, float, float, float, int );

    unsigned int        _numQuads;

    osg::Vec3Array*     _vAry;
    osg::Vec3Array*     _nAry;
};

}

#endif

// vim: set sw=4 ts=8 et ic ai:
