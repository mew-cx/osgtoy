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
 * author:      Mike Weiblen
 * copyright:   (C) 2005 3Dlabs Inc. Ltd.
 * license:     3Dlabs_license.txt
*/

// This code written to OpenGLCompilerJune082005.zip from
// http://developer.3dlabs.com/downloads/glslcompiler/index.htm
#include <glslang/Include/ShHandle.h>
#include <glslang/Public/ShaderLang.h>

#include <osgToy/GlslLint>

///////////////////////////////////////////////////////////////////////////
// TBuiltInResource represents the OpenGL implementation-dependent limits
// of the underlying hardware.
// Since GlslLint doesn't necessarily represent actual hardware, choose
// some convenient values.

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

// ShBindingTable represents the attribute bindings that are input to the
// linker.  It contains the table that the app would build with
// glBindAttribLocation().
// FUTURE: since GlslList currently doesn't do anything during linking,
// we simply contrive some values.

static ShBinding g_AttribBindings[] = {
    { "gl_Vertex", 0 },
    { "gl_Color", 10 },
    { "gl_Normal", 7 },
};

static ShBindingTable g_BoundAttribTable = { 
        sizeof(g_AttribBindings) / sizeof(ShBinding), g_AttribBindings };

static bool g_parserLibraryInitialized = false;

///////////////////////////////////////////////////////////////////////////

osgToy::GlslLint::GlslLint( Options options ) :
    _options(options), _linker(0), _uniformMap(0)
{
    if( ! g_parserLibraryInitialized )
    {
        ShInitialize();
        g_parserLibraryInitialized = true;
    }
}


osgToy::GlslLint::~GlslLint()
{
    while( !_compilerList.empty() )
    {
        ShHandle compiler = _compilerList.back();
        _compilerList.pop_back();
        ShDestruct( compiler );
    }

    if( _linker)  ShDestruct( _linker );
    if( _uniformMap )  ShDestruct( _uniformMap );
}


osgToy::GlslLint::Status
osgToy::GlslLint::compile( osg::Shader::Type type, const std::string& sourceText )
{
    int options = (_options == VERBOSE) ?  EDebugOpIntermediate : 0;
    EShLanguage lang = (type == osg::Shader::VERTEX) ? EShLangVertex : EShLangFragment;
    _infoLog = "";

    ShHandle compiler = ShConstructCompiler( lang, options );
    if( !compiler ) return ERR_COMPILER_CTOR;
    _compilerList.push_back( compiler );

    const char* text = sourceText.c_str();
    int success = ShCompile( compiler, &text, 1, EShOptNone, &g_Resources, options );
    _infoLog = ShGetInfoLog( compiler );
    if( !success ) return ERR_COMPILE;

    return SUCCESS;
}


osgToy::GlslLint::Status
osgToy::GlslLint::link()
{
    // The upstream compiler front-end only has a minimal stub for the
    // linker, so it isn't very interesting from a "lint" perspective.
    // FUTURE: implement a linker for lint to verify the consistancy of
    // shaders/uniforms/attributes.

    int options = (_options == VERBOSE) ?  EDebugOpIntermediate : 0;
    _infoLog = "";

    if( _compilerList.empty() ) return ERR_LINK;

    if( !_uniformMap ) _uniformMap = ShConstructUniformMap();
    if( !_uniformMap ) return ERR_UNIFORM_MAP;

    if( !_linker ) _linker = ShConstructLinker( EShExVertexFragment, options );
    if( !_linker ) return ERR_LINKER_CTOR;

    ShSetFixedAttributeBindings( _linker, &g_BoundAttribTable );

    int success = ShLink( _linker, &_compilerList[0], _compilerList.size(),
            _uniformMap, 0, 0 );
    _infoLog = ShGetInfoLog( _linker );
    //if( !success ) return ERR_LINK;

    return SUCCESS;
}

// vim: set sw=4 ts=8 et ic ai:
