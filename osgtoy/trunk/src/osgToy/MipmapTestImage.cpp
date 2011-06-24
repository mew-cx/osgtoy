/* file:        src/osgToy/MipmapTestImage.cpp
 * author:      Mike Weiblen
 * copyright:   (C) 2005 Michael Weiblen
 * license:     OpenSceneGraph Public License (OSGPL)
 * website:     http://mew.cx/
 * $Id$
*/

#include <osgToy/MipmapTestImage.h>

void
osgToy::MipmapTestImage::setImageData( unsigned char* data, int mipLevel, int s, int t )
{
    static unsigned char colors[][2][3] = {
    // foreground RGB     background RGB
        255, 255, 255,       0,   0,   0,
        255,   0,   0,     255, 255, 255,
          0, 255, 255,       0,   0,   0,
          0, 255,   0,     255, 255, 255,
        255,   0, 255,       0,   0,   0,
          0,   0, 255,     255, 255, 255,
        255, 255,   0,       0,   0,   0,
        255, 255, 255,     255, 255, 255,
        255,   0,   0,       0,   0,   0,
          0, 255, 255,     255, 255, 255,
          0, 255,   0,       0,   0,   0,
        255,   0, 255,     255, 255, 255,
          0,   0, 255,       0,   0,   0,
        255, 255,   0,     255, 255, 255,
    };

    int q = ( s/_checkerSize + t/_checkerSize + mipLevel ) & 0x01;
    *data     = colors[mipLevel][q][0];
    *(data+1) = colors[mipLevel][q][1];
    *(data+2) = colors[mipLevel][q][2];
    *(data+3) = 255;
}


osgToy::MipmapTestImage::MipmapTestImage( unsigned int sizeS, unsigned int sizeT ) :
    _bytesPerTexel(4), _numTexels(0), _checkerSize(8)
{
    // compute mipmap sizes & buffer offset
    MipmapDataType mipmapOffset;
    for( unsigned int s=sizeS, t=sizeT; s||t; s>>=1, t>>=1 )
    {
        if( s==0 ) s=1;
        if( t==0 ) t=1;
        _numTexels += s * t;
        _mipmapS.push_back(s);
        _mipmapT.push_back(t);
        mipmapOffset.push_back( _bytesPerTexel * _numTexels );
    }

    setImage( sizeS, sizeT, 1,
            4, GL_RGBA, GL_UNSIGNED_BYTE,
            new unsigned char[ _bytesPerTexel * _numTexels ], USE_NEW_DELETE);
    setMipmapLevels( mipmapOffset );

    // populate mipmap level colors
    for( unsigned int lvl = 0; lvl < mipmapOffset.size(); ++lvl )
    {
        unsigned char* data = getMipmapData(lvl);
        for( unsigned int t = 0; t < _mipmapT[lvl]; ++t )
        {
            for( unsigned int s = 0; s < _mipmapS[lvl]; ++s, data+=_bytesPerTexel )
            {
                setImageData( data, lvl, s, t );
            }
        }
    }
}

// vim: set sw=4 ts=8 et ic ai: 
