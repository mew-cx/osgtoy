/* file:        src/osgToy/Polyhedra.cpp
 * author:      Mike Weiblen
 * copyright:   (C) 2005 Mike Weiblen
 * license:     OpenSceneGraph Public License (OSGPL)
 * website:     http://mew.cx/
 * $Id: Polyhedra.cpp,v 1.1 2005/11/22 09:18:30 mew Exp $
*/

#include <osgToy/FacetingVisitor.h>
#include <osgToy/Polyhedra.h>

///////////////////////////////////////////////////////////////////////////

osgToy::Polyhedron::Polyhedron()
{
    setVertexArray( new osg::Vec3Array );
    setNormalArray( new osg::Vec3Array );
    setColorArray(  new osg::Vec4Array );
}

void osgToy::Polyhedron::setOverallColor( const osg::Vec4& color )
{
    osg::Vec4Array* cAry = dynamic_cast<osg::Vec4Array*>( getColorArray() );
    if( cAry )
    {
        cAry->push_back( color );
        setColorBinding( osg::Geometry::BIND_OVERALL );
    }
}

void osgToy::Polyhedron::addTristrip( const osg::Vec3& u0, const osg::Vec3& u1,
        const osg::Vec3& v0, const osg::Vec3& v1, unsigned int numQuads )
{
    osg::Vec3Array* vAry = dynamic_cast<osg::Vec3Array*>( getVertexArray() );
    int start = vAry->size();

    vAry->push_back( u0 );
    vAry->push_back( u1 );

    osg::Vec3 d0( v0 - u0 );
    osg::Vec3 d1( v1 - u1 );
    for( unsigned int i = 1; i < numQuads; ++i )
    {
        float s = float(i) / numQuads;
        vAry->push_back( u0 + d0 * s );
        vAry->push_back( u1 + d1 * s );
    }

    vAry->push_back( v0 );
    vAry->push_back( v1 );

    int count = vAry->size() - start;
    addPrimitiveSet( new osg::DrawArrays( GL_TRIANGLE_STRIP, start, count ) );
}

///////////////////////////////////////////////////////////////////////////
// http://www.cs.technion.ac.il/~gershon/EscherForReal/

osgToy::PenroseTriangle::PenroseTriangle()
{
    setOverallColor( osg::Vec4(1,0,0,1) );

    osg::Vec3 a0(  0.1,  0.2,  1.2 );
    osg::Vec3 a1(  0.1,  0.2,  0.8 );
    osg::Vec3 a2( -0.1, -0.2,  0.8 );
    osg::Vec3 a3( -0.1, -0.2,  1.2 );

    osg::Vec3 b0(  0.9892,  0.2, -0.6866 );     // rotate 120 degrees
    osg::Vec3 b1(  0.6428,  0.2, -0.4866 );
    osg::Vec3 b2(  0.7428, -0.2, -0.3134 );
    osg::Vec3 b3(  1.0892, -0.2, -0.5134 );

    osg::Vec3 c0( -1.0892,  0.2, -0.5134 );     // rotate 240 degrees
    osg::Vec3 c1( -0.7428,  0.2, -0.3134 );
    osg::Vec3 c2( -0.6428, -0.2, -0.4866 );
    osg::Vec3 c3( -0.9892, -0.2, -0.6866 );

    const unsigned int NUM_QUADS = 16;

    addTristrip( a1, a0, b0, b3, NUM_QUADS );
    addTristrip( b1, b0, c0, c3, NUM_QUADS );
    addTristrip( c1, c0, a0, a3, NUM_QUADS );

    addTristrip( a2, a1, b1, b0, NUM_QUADS );
    addTristrip( b2, b1, c1, c0, NUM_QUADS );
    addTristrip( c2, c1, a1, a0, NUM_QUADS );

    addTristrip( a0, a3, b3, b2, NUM_QUADS );
    addTristrip( b0, b3, c3, c2, NUM_QUADS );
    addTristrip( c0, c3, a3, a2, NUM_QUADS );

    addTristrip( a3, a2, b2, b1, NUM_QUADS );
    addTristrip( b3, b2, c2, c1, NUM_QUADS );
    addTristrip( c3, c2, a2, a1, NUM_QUADS );

    osgToy::FacetingVisitor::facet( *this );
}

///////////////////////////////////////////////////////////////////////////

osgToy::RhombicDodecahedron::RhombicDodecahedron()
{
    setOverallColor( osg::Vec4(0,1,0,1) );

    osg::Vec3 a0(  1,  1,  1 );
    osg::Vec3 a1(  1,  1, -1 );
    osg::Vec3 a2(  1, -1,  1 );
    osg::Vec3 a3(  1, -1, -1 );
    osg::Vec3 a4( -1,  1,  1 );
    osg::Vec3 a5( -1,  1, -1 );
    osg::Vec3 a6( -1, -1,  1 );
    osg::Vec3 a7( -1, -1, -1 );

    osg::Vec3 xp(  2,  0,  0 );
    osg::Vec3 xn( -2,  0,  0 );
    osg::Vec3 yp(  0,  2,  0 );
    osg::Vec3 yn(  0, -2,  0 );
    osg::Vec3 zp(  0,  0,  2 );
    osg::Vec3 zn(  0,  0, -2 );

    addTristrip( yp, a0, a1, xp );
    addTristrip( xp, a2, a3, yn );
    addTristrip( yn, a6, a7, xn );
    addTristrip( xn, a4, a5, yp );

    addTristrip( zp, a0, a4, yp );
    addTristrip( yp, a1, a5, zn );
    addTristrip( zn, a3, a7, yn );
    addTristrip( yn, a2, a6, zp );

    addTristrip( xp, a0, a2, zp );
    addTristrip( zp, a4, a6, xn );
    addTristrip( xn, a5, a7, zn );
    addTristrip( zn, a1, a3, xp );

    osgToy::FacetingVisitor::facet( *this );
}

///////////////////////////////////////////////////////////////////////////

osgToy::ColorOriginTetra::ColorOriginTetra()
{
    osg::Vec3 o(0,0,0);
    osg::Vec3 x(1,0,0);
    osg::Vec3 y(0,1,0);
    osg::Vec3 z(0,0,1);

    addTriangle( o, y, x );
    addTriangle( o, z, y );
    addTriangle( o, x, z );
    addTriangle( x, y, z );

    setNormalBinding( osg::Geometry::BIND_PER_PRIMITIVE );
    setColorBinding( osg::Geometry::BIND_PER_VERTEX );

    osg::Vec3Array* vAry = dynamic_cast<osg::Vec3Array*>( getVertexArray() );
    addPrimitiveSet( new osg::DrawArrays( GL_TRIANGLES, 0, vAry->size() ) );
}

void osgToy::ColorOriginTetra::addTriangle(
        const osg::Vec3& v0, const osg::Vec3& v1, const osg::Vec3& v2 )
{
    osg::Vec3Array* vAry = dynamic_cast<osg::Vec3Array*>( getVertexArray() );
    osg::Vec3Array* nAry = dynamic_cast<osg::Vec3Array*>( getNormalArray() );
    osg::Vec4Array* cAry = dynamic_cast<osg::Vec4Array*>( getColorArray() );

    osg::Vec3 norm = (v1-v0) ^ (v2-v0);
    norm.normalize();
    nAry->push_back( norm );

    vAry->push_back( v0 );  cAry->push_back( osg::Vec4(v0,1) );
    vAry->push_back( v1 );  cAry->push_back( osg::Vec4(v1,1) );
    vAry->push_back( v2 );  cAry->push_back( osg::Vec4(v2,1) );
}

///////////////////////////////////////////////////////////////////////////

osgToy::OctoStrip::OctoStrip()
{
    osg::Vec3Array* vAry = dynamic_cast<osg::Vec3Array*>( getVertexArray() );

    osg::Vec3Array* nAry = dynamic_cast<osg::Vec3Array*>( getNormalArray() );
    setNormalBinding( osg::Geometry::BIND_PER_VERTEX );

    osg::Vec4Array* cAry = dynamic_cast<osg::Vec4Array*>( getColorArray() );
    setColorBinding( osg::Geometry::BIND_PER_VERTEX );

    osg::Vec3 xp( 1, 0, 0);   osg::Vec4 red(1,0,0,1);
    osg::Vec3 xn(-1, 0, 0);   osg::Vec4 cyan(0,1,1,1);
    osg::Vec3 yp( 0, 1, 0);   osg::Vec4 green(0,1,0,1);
    osg::Vec3 yn( 0,-1, 0);   osg::Vec4 magenta(1,0,1,1);
    osg::Vec3 zp( 0, 0, 1);   osg::Vec4 blue(0,0,1,1);
    osg::Vec3 zn( 0, 0,-1);   osg::Vec4 yellow(1,1,0,1);

    vAry->push_back(zp); nAry->push_back(zp); cAry->push_back(blue);
    vAry->push_back(yp); nAry->push_back(yp); cAry->push_back(green);
    vAry->push_back(xn); nAry->push_back(xn); cAry->push_back(cyan);
    vAry->push_back(zn); nAry->push_back(zn); cAry->push_back(yellow);
    vAry->push_back(yn); nAry->push_back(yn); cAry->push_back(magenta);
    vAry->push_back(xp); nAry->push_back(xp); cAry->push_back(red);
    vAry->push_back(zp); nAry->push_back(zp); cAry->push_back(blue);
    vAry->push_back(yp); nAry->push_back(yp); cAry->push_back(green);

    addPrimitiveSet( new osg::DrawArrays( GL_TRIANGLE_STRIP, 0, vAry->size() ) );
}

///////////////////////////////////////////////////////////////////////////

osgToy::TetraStrip::TetraStrip()
{
    osg::Vec3Array* vAry = dynamic_cast<osg::Vec3Array*>( getVertexArray() );

    osg::Vec3Array* nAry = dynamic_cast<osg::Vec3Array*>( getNormalArray() );
    setNormalBinding( osg::Geometry::BIND_PER_VERTEX );

    osg::Vec4Array* cAry = dynamic_cast<osg::Vec4Array*>( getColorArray() );
    setColorBinding( osg::Geometry::BIND_PER_VERTEX );

    osg::Vec3 ppp(  1,  1,  1);   osg::Vec4 white(1,1,1,1);
    osg::Vec3 nnp( -1, -1,  1);   osg::Vec4 blue(0,0,1,1);
    osg::Vec3 pnn(  1, -1, -1);   osg::Vec4 red(1,0,0,1);
    osg::Vec3 npn( -1,  1, -1);   osg::Vec4 green(0,1,0,1);
    osg::Vec3 ppn(  1,  1, -1);   osg::Vec4 yellow(1,1,0,1);
    osg::Vec3 pnp(  1, -1,  1);   osg::Vec4 magenta(1,0,1,1);
    osg::Vec3 nnn( -1, -1, -1);   osg::Vec4 black(0,0,0,1);
    osg::Vec3 npp( -1,  1,  1);   osg::Vec4 cyan(0,1,1,1);

#if 1
    vAry->push_back(ppp); nAry->push_back(ppp); cAry->push_back(white);
    vAry->push_back(nnp); nAry->push_back(nnp); cAry->push_back(blue);
    vAry->push_back(pnn); nAry->push_back(pnn); cAry->push_back(red);
    vAry->push_back(npn); nAry->push_back(npn); cAry->push_back(green);
    vAry->push_back(ppp); nAry->push_back(ppp); cAry->push_back(white);
    vAry->push_back(nnp); nAry->push_back(nnp); cAry->push_back(blue);
#else
    vAry->push_back(ppn); nAry->push_back(ppn); cAry->push_back(yellow);
    vAry->push_back(pnp); nAry->push_back(pnp); cAry->push_back(magenta);
    vAry->push_back(nnn); nAry->push_back(nnn); cAry->push_back(black);
    vAry->push_back(npp); nAry->push_back(npp); cAry->push_back(cyan);
    vAry->push_back(ppn); nAry->push_back(ppn); cAry->push_back(yellow);
    vAry->push_back(pnp); nAry->push_back(pnp); cAry->push_back(magenta);
#endif

    addPrimitiveSet( new osg::DrawArrays( GL_TRIANGLE_STRIP, 0, vAry->size() ) );
}

// vim: set sw=4 ts=8 et ic ai:
