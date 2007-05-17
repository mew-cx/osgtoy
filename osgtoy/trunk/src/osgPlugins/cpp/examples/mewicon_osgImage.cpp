/* file:      ${IMAGE_NAME}_osgImage.cpp
 * copyright: (C) 2007 Michael Weiblen http://mew.cx/
 * license:   OpenSceneGraph Public License (OSGPL)
 * $Id: mewicon_osgImage.cpp,v 1.1 2007/03/18 23:51:22 mew Exp $
*/

#include <osg/Image>
#include "mewicon.hpp"

osg::Image* mewicon_osgImage()
{
    osg::Image* img( new osg::Image );
    img->setImage( WIDTH, HEIGHT, DEPTH, INTERNAL_FORMAT, PIXEL_FORMAT,
            DATATYPE, const_cast<unsigned char*>(PIXEL_DATA),
            osg::Image::USE_NEW_DELETE, PACKING );

    if( NUM_MIP_LEVELS && MIP_OFFSET )
    {
        osg::Image::MipmapDataType mipmapData( NUM_MIP_LEVELS );
        for( unsigned int level=0; level<NUM_MIP_LEVELS; ++level )
        {
            mipmapData[level] = MIP_OFFSET[level];
        }
        img->setMipmapLevels( mipmapData );
    }

    return img;
}

// vim: set sw=4 ts=8 et ic ai:
