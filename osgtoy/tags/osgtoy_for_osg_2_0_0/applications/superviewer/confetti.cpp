/* file:        applications/superviewer/confetti.cpp
 * author:      Mike Weiblen 2005-09-20
 * website:     http://developer.3dlabs.com/
 * copyright:   Copyright 2005 3Dlabs Inc. Ltd.
 * license:     http://developer.3dlabs.com/openGL2/downloads/oglcompiler_license.txt
 *
 * Derived from section 13.6 Particle Systems of "OpenGL Shading Language"
 * by Randi Rost; see also ogl2particle.zip available from
 * http://developer.3dlabs.com/openGL2/downloads/index.htm
*/

#include <stdlib.h>     // for rand() & RAND_MAX

#include <osg/Node>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Vec3>
#include <osg/Vec4>
#include <osg/Program>
#include <osg/Shader>
#include <osg/Point>

///////////////////////////////////////////////////////////////////////////
// This GLSL vertex shader is crafted to mesh with GL fixed-functionality
// fragment processing, so a GLSL fragment shader is not necessary.

static const char *vertexSrc = {
    "// Vertex shader for rendering a confetti cannon via a particle system\n"
    "// by Randi Rost; modified for OSG by Mike Weiblen\n"
    "// Copyright 2005 3Dlabs Inc. Ltd.\n"
    "// http://developer.3dlabs.com/openGL2/downloads/oglcompiler_license.txt \n"
    "\n"
    "uniform float osg_FrameTime;   // updated each frame by OSG \n"
    "\n"
    "attribute float StartTime;     // particle's launch time \n"
    "attribute vec3  Velocity;      // initial launch velocity \n"
    "\n"
    "void main(void)\n"
    "{\n"
    "    vec4 vert = gl_Vertex;\n"
    "    float t = mod( osg_FrameTime, 15.0 ) - StartTime;\n"
    "    if( t > 0.0 )\n"
    "    {\n"
    "        vert   += vec4( Velocity * t, 0.0 );\n"
    "        vert.z -= 4.9 * t * t;\n"
    "    }\n"
    "\n"
    "    gl_FrontColor = gl_Color;\n"
    "    gl_Position   = gl_ModelViewProjectionMatrix * vert;\n"
    "}\n"
};

///////////////////////////////////////////////////////////////////////////

#define RND(range,bias) (((float)(range) * rand() / RAND_MAX) + (bias))

osg::Node* makeConfetti( int count )
{
    osg::Geometry* geom = new osg::Geometry;

    // vertex array
    osg::Vec3Array* vtx = new osg::Vec3Array;
    geom->setVertexArray( vtx );

    // color array
    osg::Vec4Array* col = new osg::Vec4Array;
    geom->setColorArray( col );
    geom->setColorBinding( osg::Geometry::BIND_PER_VERTEX );

    // velocity array
    osg::Vec3Array* vel = new osg::Vec3Array;
    const int VELOCITY_ARRAY = 5;   // changed from 3 due to NVidia bug.
    geom->setVertexAttribArray( VELOCITY_ARRAY, vel );
    geom->setVertexAttribBinding( VELOCITY_ARRAY, osg::Geometry::BIND_PER_VERTEX );

    // starttime array
    osg::FloatArray* tim = new osg::FloatArray;
    const int STARTTIME_ARRAY = 4;
    geom->setVertexAttribArray( STARTTIME_ARRAY, tim );
    geom->setVertexAttribBinding( STARTTIME_ARRAY, osg::Geometry::BIND_PER_VERTEX );

    // a unit square of points centered at origin
    for( int i = 0; i < count; ++i )
    {
        for( int j = 0; j < count; ++j )
        {
            vtx->push_back( osg::Vec3((float)i/count-0.5f, (float)j/count-0.5f, 0) );
            col->push_back( osg::Vec4(RND(0.5,0.5), RND(0.5,0.5), RND(0.5,0.5), 1) );
            vel->push_back( osg::Vec3(RND(1,3), -RND(1,3), RND(10,0)) );
            tim->push_back( RND(10,0) );
        }
    }

    geom->addPrimitiveSet( new osg::DrawArrays( GL_POINTS, 0, vtx->getNumElements() ) );

    osg::Program* pgm = new osg::Program;
    pgm->addShader( new osg::Shader( osg::Shader::VERTEX, vertexSrc ) );
    pgm->addBindAttribLocation( "Velocity", VELOCITY_ARRAY );
    pgm->addBindAttribLocation( "StartTime", STARTTIME_ARRAY );

    osg::Point* point = new osg::Point;
    point->setSize(2);

    osg::StateSet* ss = geom->getOrCreateStateSet();
    ss->setAttributeAndModes( pgm, osg::StateAttribute::ON );
    ss->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
    ss->setAttribute(point);

    osg::Geode* geode = new osg::Geode;
    geode->addDrawable( geom );
    //TODO explicitly set the bounding volume
    geode->setCullingActive( false );
    return geode;
}

// vim: set sw=4 ts=8 et ic ai:
