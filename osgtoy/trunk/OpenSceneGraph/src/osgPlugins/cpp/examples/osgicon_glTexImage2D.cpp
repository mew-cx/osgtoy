/* file:      ${IMAGE_NAME}_glTexImage2D.cpp
 * copyright: (C) 2007 Michael Weiblen http://mew.cx/
 * license:   OpenSceneGraph Public License (OSGPL)
 * $Id: osgicon_glTexImage2D.cpp,v 1.1 2007/03/18 23:51:22 mew Exp $
*/

#include <GL/gl.h>
#include "osgicon.hpp"

void osgicon_glTexImage2D( GLint miplevel, GLint border )
{
    glTexImage2D( GL_TEXTURE_2D, miplevel, INTERNAL_FORMAT, WIDTH, HEIGHT,
            border, PIXEL_FORMAT, DATATYPE, PIXEL_DATA );
}

void osgicon_glTexImage2D( GLint border )
{
    if( NUM_MIP_LEVELS && MIP_OFFSET )
    {
        GLsizei w = WIDTH;
        GLsizei h = HEIGHT;
        const GLvoid* pixels = PIXEL_DATA;

        for( unsigned int level=0; level<NUM_MIP_LEVELS; ++level )
        {
            glTexImage2D( GL_TEXTURE_2D, level, INTERNAL_FORMAT, w, h,
                    border, PIXEL_FORMAT, DATATYPE, pixels );

            w >>= 1; if(w==0) w=1;
            h >>= 1; if(h==0) h=1;
            pixels += MIP_OFFSET[level];
        }
    }
    else
    {
        glTexImage2D( GL_TEXTURE_2D, 0, INTERNAL_FORMAT, WIDTH, HEIGHT,
                border, PIXEL_FORMAT, DATATYPE, PIXEL_DATA );
    }
}

// vim: set sw=4 ts=8 et ic ai:
