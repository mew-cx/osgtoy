// File: drawtext.frag
// Author: Mike Weiblen 2005-07-30
// Copyright (C) 2005  3Dlabs Inc. Ltd.
// All rights reserved.
// 
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
//
// An OpenGL Shading Language fragment shader to render uniform values
// as text, providing visibility into GLSL shaders.
// Inspired by Jeff Boody's printnum shader.
// Requires the verasansmono.png glyph image.
// Designed to run with fixed-function vertex processing, so no GLSL
// vertex shader is required.
//
///////////////////////////////////////////////////////////////////////////
//
// This shader manages 3 coordinate spaces:
// geometry texture coordinates: the standard normalized texcoords sent
//     as OpenGL vertex attributes.
// cell coordinates: maps rows and columns of character cells to geometry
//     texcoords; analogous to OpenGL's viewport mapping.
// glyph coordinates: for a specific ASCII character, these are coordinates
//     across a single glyph, a subset of the font texture.
//
///////////////////////////////////////////////////////////////////////////

uniform sampler2D GlyphTexture; // contains the verasansmono.png image

const vec2 CellSpan = vec2( 8.0, 3.0 ); // number of cells across texture

///////////////////////////////////////////////////////////////////////////
// Convert geometry texcoord to cell coord; i.e.: a character's row/column

vec2 getCellCoord( vec2 geomTexCoord )
{
    return floor( geomTexCoord * CellSpan );
}

///////////////////////////////////////////////////////////////////////////
// Return the texel of the ASCII character's glyph at the geometry
// texture coordinate.

vec4 readGlyphTexture( vec2 geomTexCoord, float ascii )
{
    // manifest scaling constants for the verasansmono.png glyph image.
    const vec2 _GlyphScale = vec2( 1.0/16.0, (300.0/384.0)/6.0 );

    if( (ascii < 32.0) || (ascii > 127.0) ) ascii = 127.0;
    float gn = (ascii - 32.0) * _GlyphScale.x;
    vec2 glyphOrigin = vec2( fract(gn), _GlyphScale.y * floor(gn) );
    vec2 cellTexCoord = fract( geomTexCoord * CellSpan );
    vec2 glyphTexCoord = cellTexCoord * _GlyphScale + glyphOrigin;
    return texture2D( GlyphTexture, glyphTexCoord );
}

///////////////////////////////////////////////////////////////////////////
// Return a single ASCII character from the string representation of a
// floating point value.

float Float2Ascii( float val, int index )
{
    const float minus_sign = 45.0;
    const float decimal_point = 46.0;
    const float zero = 48.0;

    // how many cells needed to the left of decimal point?
    int numCells = 0;
    if( val < 0.0 ) numCells++;
    float x = abs( val );
    do {
        x = floor( x * 0.1 );
        numCells++;
    } while( x > 0.0 );

    if( (val < 0.0) && (index == 0) ) return minus_sign;
    if( index == numCells ) return decimal_point;

    int cnt;
    float s;
    if( index < numCells )
    {
        cnt = numCells - index - 1;
        s = 0.1;
    }
    else
    {
        cnt = index - numCells;
        s = 10.0;
    }

    x = abs(val);
    for( int i = 0; i < cnt; i++ ) x *= s;
    return floor(mod(x,10.0)) + zero;
}

///////////////////////////////////////////////////////////////////////////
// For the given cell, return the ASCII character to render in that cell.
// Display each float component of a vec3 as an ASCII string.

uniform mat4 osg_ViewMatrix;
uniform int osg_FrameNumber;
uniform float osg_FrameTime;
uniform float osg_DeltaFrameTime;

float getFloatCharacter( vec2 cell )
{
    // a couple examples of interesting data to render...
    vec4 data = gl_ModelViewMatrix[3];
    //vec4 data = osg_ViewMatrix[3];
    //vec4 data = vec4( osg_FrameTime, osg_DeltaFrameTime, float(osg_FrameNumber), 0.0 );

    float val = data[ int(cell.y) ];
    float ascii = Float2Ascii( val, int(cell.x) );
    return ascii;
}

///////////////////////////////////////////////////////////////////////////
// For the given cell, return the ASCII character to render in that cell.
// Display 4 ASCII characters as packed into the components of a vec4.

uniform vec4 Word;

float getWordCharacter( vec2 cell )
{
    float ascii = floor( Word[ int(cell.x) ] );
    return ascii;
}

///////////////////////////////////////////////////////////////////////////

void main(void)
{
    vec2 texcoord = gl_TexCoord[0].xy;
    vec2 cell = getCellCoord( texcoord );

    float ascii = getFloatCharacter( cell );
    //float ascii = getWordCharacter( cell );

    vec4 texel = readGlyphTexture( texcoord, ascii );

    gl_FragColor = gl_Color * texel;
}

// vim: set sw=4 ts=8 et ic ai: 
