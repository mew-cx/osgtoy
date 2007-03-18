/* file:      src/osgPlugins/hpp/Image.cpp
 * author:    Mike Weiblen
 * copyright: (C) 2007 Michael Weiblen http://mew.cx/
 * license:   OpenSceneGraph Public License (OSGPL)
 * $Id: Image.cpp,v 1.4 2007/03/18 05:38:08 mew Exp $
*/

#include "Image.h"

#if 1
osg::Image* MakeImage()
{
#include "test.hpp"
#ifdef ARGS_OSG_SETIMAGE
    osg::Image* img( new osg::Image );
    img->setImage( ARGS_OSG_SETIMAGE );

    //glTexImage2D( ARGS_GL_TEXIMAGE2D );

    if( NUM_MIP_LEVELS && MIP_OFFSET )
    {
        osg::Image::MipmapDataType mipmapData( NUM_MIP_LEVELS );
        for( unsigned int i=0; i<NUM_MIP_LEVELS; ++i ) mipmapData[i] = MIP_OFFSET[i];
        img->setMipmapLevels( mipmapData );
    }
    return img;
#else
//#error "missing image?"
    return 0;
#endif
}
#endif


static const char* enumToStr( GLenum glenum )
{
    switch( glenum )
    {
        case GL_DEPTH_COMPONENT: return "GL_DEPTH_COMPONENT";
        case GL_RED:             return "GL_RED";
        case GL_GREEN:           return "GL_GREEN";
        case GL_BLUE:            return "GL_BLUE";
        case GL_ALPHA:           return "GL_ALPHA";
        case GL_RGB:             return "GL_RGB";
        case GL_RGBA:            return "GL_RGBA";
        case GL_LUMINANCE:       return "GL_LUMINANCE";
        case GL_LUMINANCE_ALPHA: return "GL_LUMINANCE_ALPHA";
        case GL_BYTE:            return "GL_BYTE";
        case GL_UNSIGNED_BYTE:   return "GL_UNSIGNED_BYTE";
        case GL_SHORT:           return "GL_SHORT";
        case GL_UNSIGNED_SHORT:  return "GL_UNSIGNED_SHORT";
        case GL_INT:             return "GL_INT";
        case GL_UNSIGNED_INT:    return "GL_UNSIGNED_INT";
        case GL_FLOAT:           return "GL_FLOAT";
        case GL_DOUBLE:          return "GL_DOUBLE";
    }
    return "???";
}


std::ostream& hpp::operator<< (std::ostream& fout, const osg::Image& img )
{
    fout << "// DO NOT EDIT -- generated with osgdb_hpp plugin -- http://mew.cx/" << std::endl;

//    fout << "#define ARGS_OSG_SETIMAGE "
//        "WIDTH,HEIGHT,DEPTH,INTERNAL_FORMAT,PIXEL_FORMAT,DATATYPE,"
//        "const_cast<unsigned char*>(PIXEL_DATA),"
//        "osg::Image::USE_NEW_DELETE,PACKING\n" << std::endl;

//    fout << "#define ARGS_GL_TEXIMAGE2D "
//        "GL_TEXTURE_2D,0,INTERNAL_FORMAT,WIDTH,HEIGHT,0,"
//        "PIXEL_FORMAT,DATATYPE,PIXEL_DATA\n" << std::endl;

    fout << "static const char* NAME = \""  << img.getName() << "\";" << std::endl;
    fout << "static const int WIDTH = "  << img.s() << ";" << std::endl;
    fout << "static const int HEIGHT = " << img.t() << ";" << std::endl;
    fout << "static const int DEPTH = "  << img.r() << ";" << std::endl;

    fout << "static const GLint INTERNAL_FORMAT = " << enumToStr(img.getInternalTextureFormat()) << ";" << std::endl;
    fout << "static const GLenum PIXEL_FORMAT = " << enumToStr(img.getPixelFormat()) << ";" << std::endl;
    fout << "static const GLenum DATATYPE = " << enumToStr(img.getDataType()) << ";" << std::endl;
    fout << "static const unsigned int PACKING = " << img.getPacking() << ";" << std::endl;
    fout << "static const unsigned int BITS_PER_PIXEL = " << img.getPixelSizeInBits() << ";" << std::endl;
 
    const osg::Image::MipmapDataType& mipmapData( img.getMipmapLevels() );
    const int numMips( mipmapData.size() );
    fout << "static const unsigned int NUM_MIP_LEVELS = " << numMips << ";" << std::endl;
    if( numMips )
    {
        fout << "static const unsigned int MIP_OFFSET[] = { ";
        for( int i=0; i < numMips; ++i ) fout << mipmapData[i] << ",";
        fout << "};" << std::endl;
    }
    else
    {
        fout << "static const unsigned int* MIP_OFFSET = 0;" << std::endl;
    }

    const unsigned char* data( img.data() );
    if( data )
    {
        const unsigned int numBytes( img.getTotalSizeInBytesIncludingMipmaps() );
        fout << "static const unsigned int TOTAL_NUM_BYTES = " << numBytes << ";" << std::endl;
        fout << "static const unsigned char PIXEL_DATA[TOTAL_NUM_BYTES] = { ";
        for( unsigned int i=0; i < numBytes; ++i )
        {
            if( !(i%16) ) fout << "\n\t";    // occasional linebreaks
            fout << (unsigned int)data[i] << ",";
        }
        fout << "\n};" << std::endl;
    }
    else
    {
        fout << "static const unsigned int TOTAL_NUM_BYTES = 0;" << std::endl;
        fout << "static const unsigned char* PIXEL_DATA = 0;" << std::endl;
    }

    return fout;
}

// vim: set sw=4 ts=8 et ic ai:
