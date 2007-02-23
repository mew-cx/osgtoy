/* file:      src/osgPlugins/cpp/Image.cpp
 * author:    Mike Weiblen http://mew.cx/
 * copyright: (C) 2007 Michael Weiblen
 * license:   OpenSceneGraph Public License (OSGPL)
 * $Id: Image.cpp,v 1.3 2007/02/23 16:26:55 mew Exp $
*/

#include "Image.h"

#if 1
osg::Image* MakeImage()
{
#include "test.cpp"
#ifdef ARGS_OSG_SETIMAGE
    osg::Image* img( new osg::Image );
    img->setImage( ARGS_OSG_SETIMAGE );

    //glTexImage2D( ARGS_GL_TEXIMAGE2D );

    if( NUMMIPS && MIPDATA )
    {
        osg::Image::MipmapDataType mipmapData( NUMMIPS );
        for( unsigned int i=0; i<NUMMIPS; ++i ) mipmapData[i] = MIPDATA[i];
        img->setMipmapLevels( mipmapData );
    }
    return img;
#else
//#error "missing image?"
    return 0;
#endif
}
#endif


std::ostream& cpp::operator<< (std::ostream& fout, const osg::Image& img )
{
    fout << "// DO NOT EDIT -- generated with osgdb_cpp plugin -- http://mew.cx/\n" << std::endl;

    fout << "#define ARGS_OSG_SETIMAGE "
        "WIDTH,HEIGHT,DEPTH,INTERNALFORMAT,PIXELFORMAT,DATATYPE,"
        "const_cast<unsigned char*>(PIXDATA),"
        "osg::Image::USE_NEW_DELETE,PACKING\n" << std::endl;

    fout << "#define ARGS_GL_TEXIMAGE2D "
        "GL_TEXTURE_2D,0,INTERNALFORMAT,WIDTH,HEIGHT,0,"
        "PIXELFORMAT,DATATYPE,PIXDATA\n" << std::endl;

    fout << "const char* NAME = \""  << img.getName() << "\";" << std::endl;
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
        fout << "const unsigned char PIXDATA[NUMBYTES] = { ";
        for( unsigned int i=0; i < numBytes; ++i )
        {
            if( !(i%16) ) fout << "\n\t";    // occasional linebreaks
            fout << (unsigned int)data[i] << ",";
        }
        fout << "\n};" << std::endl;
    }
    else
    {
        fout << "const unsigned int NUMBYTES = 0;" << std::endl;
        fout << "const unsigned char* PIXDATA = 0;" << std::endl;
    }

    return fout;
}

// vim: set sw=4 ts=8 et ic ai:
