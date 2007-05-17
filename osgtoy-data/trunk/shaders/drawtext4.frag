// $Id: drawtext4.frag,v 1.1 2006/06/29 06:55:06 mew Exp $
// Author: Mike Weiblen
// Copyright (C) 2006  Mike Weiblen http://mew.cx/
// Copyright (C) 2005  3Dlabs Inc. Ltd.
// All rights reserved.
// 
// An OpenGL Shading Language fragment shader to render numeric values as
// text, providing visibility into GLSL shaders.
// Inspired by Jeff Boody's printnum shader.
// Requires the verasansmono.png glyph image.
// Designed to run with fixed-function vertex processing, so no GLSL
// vertex shader is required.
//
///////////////////////////////////////////////////////////////////////////
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
//     Redistributions of source code must retain the above copyright
//     notice, this list of conditions and the following disclaimer.
// 
//     Redistributions in binary form must reproduce the above
//     copyright notice, this list of conditions and the following
//     disclaimer in the documentation and/or other materials provided
//     with the distribution.
// 
//     Neither the name of 3Dlabs Inc. Ltd. nor the names of its
//     contributors may be used to endorse or promote products derived
//     from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

///////////////////////////////////////////////////////////////////////////
// Derived from the original drawtext.frag (oct 2005), adjusted for:
// HW: NVIDIA GeForce 6200 AGP
// SW: ForceWare 90 version 91.31 released 2006-06-23
///////////////////////////////////////////////////////////////////////////

// Convert geometry texcoord to cell index; i.e.: a character's row/column

uniform vec2 CellSpan;    // number of cells across texture

ivec2 getCellIndex( vec2 geomTexCoord )
{
    return ivec2( geomTexCoord * CellSpan );
}

///////////////////////////////////////////////////////////////////////////
// Return the texel of an ASCII character's glyph at the given geometry
// texture coordinate.

uniform sampler2D GlyphTexSampler; // contains the verasansmono.png image

vec4 readGlyphTexture( vec2 geomTexCoord, int ascii )
{
    // glyph scaling constants for the verasansmono.png image.
    const vec2 glyScale = vec2( 1.0/16.0, (300.0/384.0)/6.0 );

    vec2 cellTexCoord = fract( geomTexCoord * CellSpan );
    if( (ascii < 32) || (ascii > 127) ) ascii = 127;
    float glyNum = float(ascii - 32) * glyScale.x;
    vec2 glyTexOrigin = vec2( fract(glyNum), glyScale.y * floor(glyNum) );
    vec2 glyTexCoord = cellTexCoord * glyScale + glyTexOrigin;
    return texture2D( GlyphTexSampler, glyTexCoord );
}

///////////////////////////////////////////////////////////////////////////
// Return a single ASCII character from the decimal expansion of a float.
// (change "base" to render as binary, decimal, or hexadecimal)

// restructured for "error C5051: profile does not support conditional returns"

const float base = 10.;

int findRadixPos( float val )
{
    // compute 'radixPos', the cell position of the radix point
    int radixPos = 0;
    if( val < 0. )
    {
        ++radixPos;                     // leave room for minus sign
        val = abs(val);
    }

    do {
        val = floor( val / base );
        ++radixPos;
    } while( val > 0. );
    return radixPos;
}

int Float2Ascii( float val, int pos )
{
    int ascii = 0;

    if( (pos == 0) && (val < 0.) )
    {
        ascii = 45;   // minus sign '-'
    }
    else
    {
        int radixPos = findRadixPos(val);
        if( pos == radixPos )
        {
            ascii = 46;                // radix point '.'
        }
        else
        {
            int exp = pos - radixPos;
            if( pos < radixPos ) ++exp;

#if 0 //[ accuracy issues w/ pow()
            val = abs(val) * pow( base, exp );
#else // workaround:
            val = abs(val);
            while( exp < 0 ) { val /= base; ++exp; }
            while( exp > 0 ) { val *= base; --exp; }
#endif //]

            int digit = int( mod( val, base ) );
            if( base>10. && digit>9 ) digit += 39;  // offset into alphabet
            ascii = digit + 48;                     // add ascii zero '0'
        }
    }
    return ascii;
}

///////////////////////////////////////////////////////////////////////////
// Utility functions which, when given a cell position, return a single
// ASCII character to be rendered in that cell.
///////////////////////////////////////////////////////////////////////////

// Display each vec4 component as a decimal ASCII string, analogous to
// printf( "%f", data[cell.y] )

int getFloatCharacter( ivec2 cell, vec4 data )
{
#if 0 //[ "error C1011: cannot index a non-array value"
    return Float2Ascii( data[cell.y], cell.x );
#else // workaround:
    int i = cell.y;
    float val;

    if      ( i >= 4 ) val = 0.;
    else if ( i >= 3 ) val = data[3];
    else if ( i >= 2 ) val = data[2];
    else if ( i >= 1 ) val = data[1];
    else if ( i >= 0 ) val = data[0];
    else val = 0.;

    return Float2Ascii( val, cell.x );
#endif //]
}

///////////////////////////////////////////////////////////////////////////
// Display each vec4 component as a single ASCII character, analogous to
// printf( "%4c", data[0], data[1], data[2], data[3] )

int getWordCharacter( ivec2 cell, vec4 data )
{
#if 0 //[ "error C1011: cannot index a non-array value"
    return int( data[cell.x] );
#else // workaround:
    int i = cell.x;
    float ascii;

    if      ( i >= 4 ) ascii = 0.;
    else if ( i >= 3 ) ascii = data[3];
    else if ( i >= 2 ) ascii = data[2];
    else if ( i >= 1 ) ascii = data[1];
    else if ( i >= 0 ) ascii = data[0];
    else ascii = 0.;

    return int(ascii);
#endif //]
}

///////////////////////////////////////////////////////////////////////////
// The main() function configures what and how to display the desired data.
// Below are some simple examples, use as you wish.
///////////////////////////////////////////////////////////////////////////

// render the vec4 "Word" as 4 ascii characters

#if 0 //[

uniform vec4 Word;

void main(void)
{
    vec2 geomTexCoord = gl_TexCoord[0].xy;
    ivec2 cellIndex = getCellIndex( geomTexCoord );
    int ascii = getWordCharacter( cellIndex, Word );
    vec4 texel = readGlyphTexture( geomTexCoord, ascii );
    gl_FragColor = gl_Color * texel;
}

#endif //]

///////////////////////////////////////////////////////////////////////////

#if 1 //[

// render a vec4 as 4 lines of printf-like numeric strings.

uniform mat4 osg_ViewMatrix;
uniform int osg_FrameNumber;
uniform float osg_FrameTime;
uniform float osg_DeltaFrameTime;

void main(void)
{
    vec4 data;

    // some examples of interesting data to render...
    data = gl_ModelViewMatrix[3];
    //data = osg_ViewMatrix[3];
    //data = gl_LightSource[0].position;
    //data = gl_LightSource[0].diffuse;
    //data = vec4( osg_FrameTime, osg_DeltaFrameTime, float(osg_FrameNumber), 0.0 );

    vec2 geomTexCoord = gl_TexCoord[0].xy;
    ivec2 cellIndex = getCellIndex( geomTexCoord );
    int ascii = getFloatCharacter( cellIndex, data );
    vec4 texel = readGlyphTexture( geomTexCoord, ascii );
    gl_FragColor = gl_Color * texel;
}

#endif //]

// vim: set sw=4 ts=8 et ic ai:
