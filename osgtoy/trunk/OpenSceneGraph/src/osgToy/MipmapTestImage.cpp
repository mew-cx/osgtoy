/* file:        src/osgToy/MipmapTestImage.cpp
 * author:      Mike Weiblen 2005-08-26
 * copyright:   (C) 2005 Michael Weiblen
 * license:     OpenSceneGraph Public License (OSGPL)
 * website:     http://mew.cx/osg/
 * $Id: MipmapTestImage.cpp,v 1.1 2005/08/28 04:34:23 mew Exp $
*/

#include <osgToy/MipmapTestImage>

osgToy::MipmapTestImage::MipmapTestImage( unsigned int sizeS, unsigned int sizeT ) :
    _bytesPerTexel(4), _numTexels(0), _gridSize(8)
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

    // populate mipmap levels
    for( unsigned int lvl = 0; lvl < mipmapOffset.size(); ++lvl )
    {
        unsigned char* ptr = getMipmapData(lvl);
        for( unsigned int t = 0; t < _mipmapT[lvl]; ++t )
        {
            for( unsigned int s = 0; s < _mipmapS[lvl]; ++s )
            {
                int q = ( s/_gridSize + t/_gridSize + lvl ) & 0x01;
                *ptr++ = s_color[lvl][q][0];
                *ptr++ = s_color[lvl][q][1];
                *ptr++ = s_color[lvl][q][2];
                *ptr++ = 255;
            }
        }
    }
}

unsigned char
osgToy::MipmapTestImage::s_color[][2][3] = {
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

// vim: set sw=4 ts=8 et ic ai: 
