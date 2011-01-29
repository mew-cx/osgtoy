/* file:        include/osgToy/MipmapTestImage
 * author:      Mike Weiblen
 * copyright:   (C) 2005 Michael Weiblen
 * license:     OpenSceneGraph Public License (OSGPL)
 * website:     http://mew.cx/
 * $Id$
*/

#ifndef OSGTOY_MIPMAPTESTIMAGE
#define OSGTOY_MIPMAPTESTIMAGE 1

#include <osg/Image>
#include <osg/Vec4>
#include <osgToy/Export.h>

namespace osgToy {

class OSGTOY_EXPORT MipmapTestImage : public osg::Image
{
public:
    MipmapTestImage( unsigned int sizeS = 256, unsigned int sizeT = 256 );

protected:
    virtual ~MipmapTestImage() {}
    virtual void setImageData( unsigned char* data, int mipLevel, int s, int t );

    int _bytesPerTexel;
    int _numTexels;
    int _checkerSize;
    std::vector< unsigned int > _mipmapS;
    std::vector< unsigned int > _mipmapT;
};

}

#endif

// vim: set sw=4 ts=8 et ic ai: 
