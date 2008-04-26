-- construct a SuperShape using the osgToy .lua plugin
-- see http://astronomy.swin.edu.au/~pbourke/surfaces/supershape3d/

return osgToy.SuperShape3D {
    name =	"five point star",
    resolution = 102,
    ss1_m =	5,
    ss1_a =	1,
    ss1_b =	1,
    ss1_n1 =	0.1,
    ss1_n2 =	1.7,
    ss1_n3 =	1.7,
    ss2_m =	1,
    ss2_a =	1,
    ss2_b =	1,
    ss2_n1 =	0.3,
    ss2_n2 =	0.5,
    ss2_n3 =	0.5,
}
