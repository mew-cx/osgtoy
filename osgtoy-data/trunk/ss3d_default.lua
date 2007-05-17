-- construct a SuperShape using the osgToy .lua plugin
-- see http://astronomy.swin.edu.au/~pbourke/surfaces/supershape3d/

return osgToy.SuperShape3D {
    name =	"a default shape",
    resolution = 200,
    ss1_m =	7,
    ss1_a =	1,
    ss1_b =	1,
    ss1_n1 =	20.45,
    ss1_n2 =	-0.33,
    ss1_n3 =	-3.54,
    ss2_m =	6,
    ss2_a =	1,
    ss2_b =	1,
    ss2_n1 =	-0.96,
    ss2_n2 =	4.46,
    ss2_n3 =	0.52,
}
