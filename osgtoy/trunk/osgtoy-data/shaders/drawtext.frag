// File: drawtext.frag
// Author: Mike Weiblen 2005-09-22
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

int Float2Ascii( float val, int index )
{
    const int minus_sign = 45;
    const int decimal_point = 46;
    const int zero = 48;

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
    return int(mod(x,10.0)) + zero;
}

///////////////////////////////////////////////////////////////////////////
// Return the ASCII character to be rendered at the given geomTexCoord:
// Display each vec4 component as a decimal ASCII string, analogous to
// printf( "%f", data[cell.y] );

int getFloatCharacter( ivec2 cell, vec4 data )
{
    return Float2Ascii( data[cell.y], cell.x );
}

///////////////////////////////////////////////////////////////////////////
// Return the ASCII character to be rendered at the given geomTexCoord:
// Display each vec4 component as a single ASCII character, analogous to
// printf( "%4c", data[0], data[1], data[2], data[3] );

int getWordCharacter( ivec2 cell, vec4 data )
{
    return int( data[cell.x] );
}

///////////////////////////////////////////////////////////////////////////
// The main() function configures what and how to display.
// TODO: split these main() functions into separate files.
///////////////////////////////////////////////////////////////////////////

#if 0 //[

// File: drawWordVec4.frag

ivec2 getCellIndex( vec2 geomTexCoord );
int getWordCharacter( vec2 geomTexCoord, vec4 data );
vec4 readGlyphTexture( vec2 geomTexCoord, int ascii );

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

// File: drawFloatVec4.frag

ivec2 getCellIndex( vec2 geomTexCoord );
int getFloatCharacter( vec2 geomTexCoord, vec4 data );
vec4 readGlyphTexture( vec2 geomTexCoord, int ascii );

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
