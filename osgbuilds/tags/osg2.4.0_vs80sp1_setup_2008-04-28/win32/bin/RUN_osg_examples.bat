@echo off
title osg_examples

set PATH=%OSG_PATH%;%PATH%

echo This script runs a series of OpenSceneGraph example applications.
echo Press ESCAPE to terminate one example and proceed to the next.
echo To terminate the entire sequence, close this console window.

@echo on

osgversion
osglogo
osgshaders
osgfxbrowser dumptruck.osg

osgviewer cow.osg
osgviewer --stereo cessna.osg
osgviewer spaceship.osg
osgviewer osgcool.osg
osgviewer cube_mapped_torus.osg

osganimate
osgautotransform
osgbillboard
osgblendequation cessnafire.osg
osgcallback cow.osg
osgcameragroup cessnafire.osg
osgclip cow.osg
osgcubemap cessna.osg
osgdepthshadow
osgdistortion cow.osg
osgforest
osggeometry
osghangglide
osghud dumptruck.osg
osgimpostor
osgkeyboard
osgkeyboardmouse cessna.osg
osglauncher
osglight glider.osg
osglightpoint
osglogicop glider.osg
osgmotionblur cow.osg
osgmultitexture cessnafire.osg
osgoccluder glider.osg
osgparametric
osgparticle
osgparticleeffects
osgpick fountain.osg
osgplanets
osgpointsprite
osgprerender dumptruck.osg
osgprerendercubemap
osgreflect cessna.osg
osgscalarbar
osgscribe cow.osg
osgsequence cow.osg dumptruck.osg cessna.osg glider.osg
osgshaderterrain
osgshadowtexture cessna.osg
osgshape
osgsimplifier --ratio 0.2 --max-error 0.05 cessna.osg
osgsimulation
osgspheresegment
osgspotlight
osgstereoimage Images/dog_left_eye.jpg Images/dog_right_eye.jpg
osgteapot
osgtesselate
osgtext
osgtexture1D dumptruck.osg
osgtexture2D
osgtexture3D
osgtexturerectangle
osgvertexprogram
osgvolume Images/osg256.png
osgwindows cow.osg

osgviewer glsl_simple.osg
osgviewer glsl_confetti.osg
osgviewer glsl_mandelbrot.osg
osgviewer glsl_julia.osg

