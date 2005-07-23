/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2005 Robert Osfield 
 *
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * OpenSceneGraph Public License for more details.
*/

/* file:        src/osgToy/GlslLint.cpp
 * author:      Mike Weiblen http://mew.cx/ 2005-07-22
 * copyright:   (C) 2005 Michael Weiblen
 * license:     OpenSceneGraph Public License (OSGPL)
*/

#include <glslang/Include/ShHandle.h>
#include <glslang/Public/ShaderLang.h>

#include <osgToy/GlslLint>

///////////////////////////////////////////////////////////////////////////
// TBuiltInResource represents the OpenGL implementation-dependent limits
// of the underlying hardware.
// Since GlslLint doesn't necessarily represent actual hardware, we can use
// any convenient values.

static TBuiltInResource g_Resources = {
    32,    // GL_MAX_LIGHTS
    6,     // GL_MAX_CLIP_PLANES
    32,    // GL_MAX_TEXTURE_UNITS
    32,    // GL_MAX_TEXTURE_COORDS
    64,    // GL_MAX_VERTEX_ATTRIBS
    4096,  // GL_MAX_VERTEX_UNIFORM_COMPONENTS
    64,    // GL_MAX_VARYING_FLOATS
    32,    // GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS
    32,    // GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS
    32,    // GL_MAX_TEXTURE_IMAGE_UNITS
    4096,  // GL_MAX_FRAGMENT_UNIFORM_COMPONENTS
    32     // GL_MAX_DRAW_BUFFERS
};

///////////////////////////////////////////////////////////////////////////
// ShBindingTable represents the attribute bindings that are input to the
// linker.  It contains the table that the app would build with
// glBindAttribLocation().
// Temporarily, since GlslList currently doesn't do anything during linking,
// we simply contrive some values.

static ShBinding g_AttribBindings[] = {
    { "gl_Vertex", 0 },
    { "gl_Color", 10 },
    { "gl_Normal", 7 },
};

static ShBindingTable g_BoundAttribTable = { 
        sizeof(g_AttribBindings) / sizeof(ShBinding), g_AttribBindings };

///////////////////////////////////////////////////////////////////////////

osgToy::GlslLint::GlslLint( Options options ) :
    _options(options), _status(SUCCESS)
{
    ShInitialize();
}


osgToy::GlslLint::~GlslLint()
{
    for( unsigned int i = 0; i < _compilerList.size(); i++ )
    {
        //ShDestruct( compilers[i] );
    }

    ShDestruct( _linker );
    ShDestruct( _uniformMap );
}


osgToy::GlslLint::Status osgToy::GlslLint::compile( osg::Shader::Type type, const std::string& sourceText )
{
    EShLanguage lang = (type == osg::Shader::VERTEX) ? EShLangVertex : EShLangFragment;
    int options = (_options == VERBOSE) ?  EDebugOpIntermediate : 0;

    ShHandle cplr = ShConstructCompiler( lang, options );
    if( !cplr ) return ERR_COMPILER_CTOR;

    const char* text = sourceText.c_str();
    int success = ShCompile( cplr, &text, 1, EShOptNone, &g_Resources, options );
    if( !success ) return ERR_COMPILE;

    _compilerList.push_back( cplr );

    printf( "#### BEGIN COMPILER INFO LOG ####\n" );
    puts( ShGetInfoLog( cplr ) );
    printf( "#### END COMPILER INFO LOG ####\n\n" );

    return SUCCESS;
}


osgToy::GlslLint::Status osgToy::GlslLint::link()
{
    int options = (_options == VERBOSE) ?  EDebugOpIntermediate : 0;

    if( _compilerList.empty() ) return ERR_LINK;

    //ShHandle compilers[10];

    _uniformMap = ShConstructUniformMap();
    if( !_uniformMap ) return ERR_UNIFORM_MAP;

    _linker = ShConstructLinker( EShExVertexFragment, options );
    if( !_linker ) return ERR_LINKER_CTOR;

    ShSetFixedAttributeBindings( _linker, &g_BoundAttribTable );

    //int success = ShLink( _linker, compilers, _compilerList.size(), uniformMap, 0, 0 );
    //THIS ALWAYS FAILS: if( !success ) return ERR_LINK;

    printf( "#### BEGIN LINKER INFO LOG ####\n" );
    puts( ShGetInfoLog( _linker ) );
    printf( "#### END LINKER INFO LOG ####\n\n" );


    return SUCCESS;
}


// vim: set sw=4 ts=8 et ic ai:
