/* file:        applications/superviewer/StaticImage.cpp
 * author:      Mike Weiblen 2005-09-10
*/

#include <osg/Image>

#if 0 //[
#include <osg/Node>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Vec2>
#include <osg/Vec3>
#include <osg/Vec4>
#include <osg/Texture2D>
#endif //]

///////////////////////////////////////////////////////////////////////////

osg::Image* makeStaticImage()
{
//#include "pnm/colorbars.png.h"
//#include "pnm/verasansmono.png.h"
#include "pnm/X32.png.h"

    osg::Image* img = new osg::Image();
    img->setImage( width, height, 1,
            pixelFormat, pixelFormat, dataType,
            const_cast<unsigned char*>(dataBuffer), osg::Image::NO_DELETE );
    return img;
}

// vim: set sw=4 ts=8 et ic ai: 
