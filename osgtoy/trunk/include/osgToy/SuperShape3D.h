/* file:        include/osgToy/SuperShape3D
 * author:      Mike Weiblen
 * copyright:   (C) 2005 Michael Weiblen
 * license:     OpenSceneGraph Public License (OSGPL)
 * website:     http://mew.cx/
 * $Id: SuperShape3D,v 1.2 2005/11/22 09:19:00 mew Exp $
 *
 * OSG code derived from Jason McVeigh's OpenSceneGraph Tutorials:
 * http://www.tersesolutions.net/osgt/
 * http://www.tersesolutions.net/osgt/index.php?Tutorial=osgtSuperShape
 *
 * see also:
 * http://astronomy.swin.edu.au/~pbourke/surfaces/supershape3d/
*/

#ifndef OSGTOY_SUPERSHAPE3D
#define OSGTOY_SUPERSHAPE3D 1

#include <osgToy/Export.h>
#include <osg/Geometry>
#include <osg/Vec3>

namespace osgToy {

class OSGTOY_EXPORT SuperShape3D : public osg::Geometry
{
public:
    SuperShape3D(
        float ss1_m = 7,        // some convenient defaults for demo
        float ss1_a = 1,
        float ss1_b = 1,
        float ss1_n1 = 20.45,
        float ss1_n2 = -0.33,
        float ss1_n3 = -3.54,
        float ss2_m = 6,
        float ss2_a = 1,
        float ss2_b = 1,
        float ss2_n1 = -0.96,
        float ss2_n2 = 4.46,
        float ss2_n3 = 0.52,
        int resolution = 200 );

private :
    float SS1( float );
    float SS2( float );
    void generate();

    float _ss1_m, _ss1_a, _ss1_b, _ss1_n1, _ss1_n2, _ss1_n3;
    float _ss2_m, _ss2_a, _ss2_b, _ss2_n1, _ss2_n2, _ss2_n3;
    int _resolution;
};

}

#endif

// vim: set sw=4 ts=8 et ic ai:
