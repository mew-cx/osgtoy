/* file:        src/osgToy/SuperShape3D.cpp
 * author:      Mike Weiblen 2005-04-19
 * copyright:   (C) 2005 Michael Weiblen
 * license:     OpenSceneGraph Public License (OSGPL)
 * website:     http://mew.cx/osg/
 * $Id: SuperShape3D.cpp,v 1.1 2005/06/06 22:10:38 mew Exp $
 *
 * OSG code derived from Jason McVeigh's OpenSceneGraph Tutorials:
 * http://www.tersesolutions.net/osgt/
 * http://www.tersesolutions.net/osgt/index.php?Tutorial=osgtSuperShape
 *
 * see also:
 * http://astronomy.swin.edu.au/~pbourke/surfaces/supershape3d/
*/

#ifdef WIN32
#include <limits>
#endif
#include <osg/Notify>
#include <osg/Math>
#include <osgToy/SuperShape3D>

///////////////////////////////////////////////////////////////////////////

osgToy::SuperShape3D::SuperShape3D(
    float ss1_m,  float ss1_a,  float ss1_b,
    float ss1_n1, float ss1_n2, float ss1_n3,
    float ss2_m,  float ss2_a,  float ss2_b,
    float ss2_n1, float ss2_n2, float ss2_n3,
    int resolution ) :
        _ss1_m(ss1_m),
        _ss1_a(ss1_a),
        _ss1_b(ss1_b),
        _ss1_n1(ss1_n1),
        _ss1_n2(ss1_n2),
        _ss1_n3(ss1_n3),

        _ss2_m(ss2_m),
        _ss2_a(ss2_a),
        _ss2_b(ss2_b),
        _ss2_n1(ss2_n1),
        _ss2_n2(ss2_n2),
        _ss2_n3(ss2_n3),

        _resolution( resolution )
{
    generate();
}

osgToy::SuperShape3D::SuperShape3D( const SuperShape3D& ss, const osg::CopyOp& copyop ) :
        osg::Geometry(ss,copyop)
{
}

float osgToy::SuperShape3D::SS1( float T )
{
    return powf(powf(fabsf(cosf(_ss1_m*T/4)/_ss1_a), _ss1_n2)
            + powf(fabsf(sinf(_ss1_m*T/4)/_ss1_b), _ss1_n3), 1.0f/_ss1_n1 );
}

float osgToy::SuperShape3D::SS2( float T )
{
    return powf(powf(fabsf(cosf(_ss2_m*T/4)/_ss2_a), _ss2_n2)
            + powf(fabsf(sinf(_ss2_m*T/4)/_ss2_b), _ss2_n3), 1.0f/_ss2_n1 );
}

void osgToy::SuperShape3D::addTriangle( const osg::Vec3& v0, const osg::Vec3& v1, const osg::Vec3& v2 )
{
    osg::Vec3 norm = (v1-v0) ^ (v2-v0);
    norm.normalize();

    _vAry->push_back( v0 );  _nAry->push_back( norm );
    _vAry->push_back( v1 );  _nAry->push_back( norm );
    _vAry->push_back( v2 );  _nAry->push_back( norm );
}

void osgToy::SuperShape3D::generate(void)
{
    const float lon_step =  2 * osg::PI / _resolution;
    const float lat_step =  osg::PI / _resolution;
    const float epsilon = std::numeric_limits<float>::epsilon();

    _vAry = new osg::Vec3Array;
    setVertexArray( _vAry );

    _nAry = new osg::Vec3Array;
    setNormalArray( _nAry );
    setNormalBinding( osg::Geometry::BIND_PER_VERTEX );

    _cAry = new osg::Vec4Array;
    setColorArray( _cAry );
    setColorBinding( osg::Geometry::BIND_OVERALL );
    _cAry->push_back( osg::Vec4( 1, 1, 1, 1 ) );

    for( int lat_count = 0; lat_count <_resolution; ++lat_count )
    {
        float phi1 = static_cast<float>(lat_count * lat_step - osg::PI_2);
        float phi2 = phi1 + lat_step;

        for( int lon_count = 0; lon_count < _resolution; ++lon_count )
        {
            float theta1 = static_cast<float>(lon_count * lon_step - osg::PI);
            float theta2 = theta1 + lon_step;

            bool tooSmall = false;

            float r1_1 = SS1(theta1);
            if(fabs(r1_1) < epsilon) tooSmall = true;

            float r2_1 = SS2(phi1);
            if(fabs(r2_1) < epsilon) tooSmall = true;

            float r1_2 = SS1(theta2);
            if(fabs(r1_2) < epsilon) tooSmall = true;

            float r2_2 = SS2(phi2);
            if(fabs(r2_2) < epsilon) tooSmall = true;

            if( !tooSmall )
            {
                r1_1 = 1.0f / r1_1;
                r1_2 = 1.0f / r1_2;
                r2_1 = 1.0f / r2_1;
                r2_2 = 1.0f / r2_2;

                osg::Vec3 pa = osg::Vec3(r1_1*cosf(theta1)*r2_1*cosf(phi1), r1_1*sinf(theta1)*r2_1*cosf(phi1), r2_1*sinf(phi1));
                osg::Vec3 pb = osg::Vec3(r1_2*cosf(theta2)*r2_1*cosf(phi1), r1_2*sinf(theta2)*r2_1*cosf(phi1), r2_1*sinf(phi1));
                osg::Vec3 pc = osg::Vec3(r1_2*cosf(theta2)*r2_2*cosf(phi2), r1_2*sinf(theta2)*r2_2*cosf(phi2), r2_2*sinf(phi2));
                osg::Vec3 pd = osg::Vec3(r1_1*cosf(theta1)*r2_2*cosf(phi2), r1_1*sinf(theta1)*r2_2*cosf(phi2), r2_2*sinf(phi2));

                if((pa - pb).length() > epsilon && (pa - pc).length() > epsilon)
                    addTriangle( pa, pb, pc );

                if((pc - pd).length() > epsilon && (pc - pa).length() > epsilon)
                    addTriangle( pc, pd, pa );
            }
        }
    }

    addPrimitiveSet( new osg::DrawArrays( GL_TRIANGLES, 0, _vAry->size() ) );
    dirtyDisplayList();

    _vAry = _nAry = 0;  // prevent accidental misuse
    _cAry = 0;
}

/* vim: set sw=4 ts=8 et ic ai: */
/*EOF*/
