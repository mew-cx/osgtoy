/* file:      src/osgPlugins/cpp/Image.cpp
 * author:    Mike Weiblen http://mew.cx/
 * copyright: (C) 2007 Michael Weiblen
 * license:   OpenSceneGraph Public License (OSGPL)
 * $Id 2007-02-03 $
*/

#include "Image.h"

#if 1 //[

osg::Image* makeOsgImage(
    const int width,
    const int height,
    const int depth,
    const GLint internalFormat,
    const GLenum pixelFormat,
    const GLenum dataType,
    const unsigned int packing,
    const unsigned int pixBits,
    const unsigned int numMips,
    const unsigned int* mipData,
    const unsigned int numBytes,
    const unsigned char* pixBytes )
{
    osg::Image* img( new osg::Image );

    img->setImage( width, height, depth, internalFormat, pixelFormat,
            dataType, const_cast<unsigned char*>(pixBytes),
            osg::Image::USE_NEW_DELETE, packing );

    if( numMips && mipData )
    {
        osg::Image::MipmapDataType mipmapData( numMips );
        for( unsigned int i=0; i<numMips; ++i ) mipmapData[i] = mipData[i];
        img->setMipmapLevels( mipmapData );
    }
    return img;
}


osg::Image* MakeImage()
{
#include "z.cpp"
#ifdef MAKE_IMAGE
    return MAKE_IMAGE(makeOsgImage);
#undef MAKE_IMAGE
#else
//#error "missing image?"
    return 0;
#endif
}

#endif //]


std::ostream& cpp::operator<< (std::ostream& fout, const osg::Image& img )
{
    fout << "#define MAKE_IMAGE(func) "
        "func(WIDTH,HEIGHT,DEPTH,INTERNALFORMAT,PIXELFORMAT,DATATYPE,"
        "PACKING,PIXBITS,NUMMIPS,MIPDATA,NUMBYTES,PIXBYTES)" << std::endl;

    fout << "const int WIDTH = "  << img.s() << ";" << std::endl;
    fout << "const int HEIGHT = " << img.t() << ";" << std::endl;
    fout << "const int DEPTH = "  << img.r() << ";" << std::endl;

    fout << "const GLint INTERNALFORMAT = 0x" << std::hex << img.getInternalTextureFormat() << std::dec << ";" << std::endl;
    fout << "const GLenum PIXELFORMAT = 0x" << std::hex << img.getPixelFormat() << std::dec << ";" << std::endl;
    fout << "const GLenum DATATYPE = 0x" << std::hex << img.getDataType() << std::dec << ";" << std::endl;
    fout << "const unsigned int PACKING = " << img.getPacking() << ";" << std::endl;
    fout << "const unsigned int PIXBITS = " << img.getPixelSizeInBits() << ";" << std::endl;
 
    const osg::Image::MipmapDataType& mipmapData( img.getMipmapLevels() );
    const int numMips( mipmapData.size() );
    fout << "const unsigned int NUMMIPS = " << numMips << ";" << std::endl;
    if( numMips )
    {
        fout << "const unsigned int MIPDATA[] = { ";
        for( int i=0; i < numMips; ++i ) fout << mipmapData[i] << ",";
        fout << "};" << std::endl;
    }
    else
    {
        fout << "const unsigned int* MIPDATA = 0;" << std::endl;
    }

    const unsigned char* data( img.data() );
    if( data )
    {
        const unsigned int numBytes( img.getTotalSizeInBytesIncludingMipmaps() );
        fout << "const unsigned int NUMBYTES = " << numBytes << ";" << std::endl;
        fout << "const unsigned char PIXBYTES[NUMBYTES] = { " << std::hex;
        for( unsigned int i=0; i < numBytes; ++i )
        {
            if( !(i%16) ) fout << std::endl << "\t";  // arbitrary linebreaks
            fout << "0x" << (unsigned int)data[i] << ",";
        }
        fout << std::endl << std::dec << "};" << std::endl;
    }
    else
    {
        fout << "const unsigned int NUMBYTES = 0;" << std::endl;
        fout << "const unsigned char* PIXBYTES = 0;" << std::endl;
    }

    return fout;
}

// vim: set sw=4 ts=8 et ic ai:
