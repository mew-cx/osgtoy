/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2005 Robert Osfield 
 *
 * This application is open source and may be redistributed and/or modified   
 * freely and without restriction, both in commericial and non commericial applications,
 * as long as this copyright notice is maintained.
 * 
 * This application is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

/* file:        examples/osgVRPNviewer/osgVRPNviewer.cpp
 * author:      Mike Weiblen mew@mew.cx
 * copyright:   (C) 2003-2005 Michael Weiblen
 * license:     OpenSceneGraph Public License (OSGPL)
 * $Id 2005-11-01 $
 *
 * references:  http://www.openscenegraph.org/
 *              http://www.vrpn.org/
 *              http://www.mew.cx/
*/

#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/Group>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include <osgProducer/Viewer>

#include "osgVRPN/Tracker.h"
#include "osgVRPN/TrackerTransform.h"
#include "osgVRPN/TrackerManipulator.h"

#define CAMERA_TRACKER_NAME "Tracker0@localhost"
//#define NODE_TRACKER_NAME "Tracker0@localhost"

///////////////////////////////////////////////////////////////////////////

osg::Node* buildScene( osgProducer::Viewer* viewer )
{
    osg::Group* scene = new osg::Group;

    osg::Geode* geode = new osg::Geode;
    geode->addDrawable(new osg::ShapeDrawable(new osg::Cone(osg::Vec3(0,0,0),1,2)));
#if defined(NODE_TRACKER_NAME)
    osgVRPN::TrackerTransform* xform = new osgVRPN::TrackerTransform;
    xform->setTracker( new osgVRPN::Tracker( NODE_TRACKER_NAME ) );
    xform->addChild( geode );
    scene->addChild( xform );
#else
    scene->addChild( geode );
#endif

    geode = new osg::Geode;
    geode->addDrawable(new osg::ShapeDrawable(new osg::Box(osg::Vec3(0,0,-2),100,100,0.1)));
    osg::Texture2D* tex0 = new osg::Texture2D;
    tex0->setImage( osgDB::readImageFile( "Images/lz.rgb" ) );
    geode->getOrCreateStateSet()->setTextureAttributeAndModes( 0, tex0, osg::StateAttribute::ON );
    scene->addChild( geode );

    geode = new osg::Geode;
    geode->addDrawable(new osg::ShapeDrawable(new osg::Box(osg::Vec3(0,0,0),3,3,0.1)));
    geode->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(40,0,0),5)));
    geode->addDrawable(new osg::ShapeDrawable(new osg::Cone(osg::Vec3(0,40,0),2,9)));
    geode->addDrawable(new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(-40,0,0),3,7)));
    geode->addDrawable(new osg::ShapeDrawable(new osg::Box(osg::Vec3(0,-40,0),7,7,7)));
    scene->addChild( geode );

    return scene;
}

int main( int argc, char **argv )
{
    osg::ArgumentParser arguments(&argc,argv);
    arguments.getApplicationUsage()->setApplicationName(arguments.getApplicationName());
    arguments.getApplicationUsage()->setDescription(arguments.getApplicationName()+" demonstrates osgVRPN");
    arguments.getApplicationUsage()->setCommandLineUsage(arguments.getApplicationName()+" [options]");
    arguments.getApplicationUsage()->addCommandLineOption("-h or --help","Display this information");

    osgProducer::Viewer viewer(arguments);
    viewer.setUpViewer( osgProducer::Viewer::STANDARD_SETTINGS );

    viewer.getUsage( *arguments.getApplicationUsage() );
    if (arguments.read("-h") || arguments.read("--help"))
    {
        arguments.getApplicationUsage()->write(std::cout);
        return 1;
    }

#if defined(CAMERA_TRACKER_NAME)
    osgVRPN::TrackerManipulator* manip = new osgVRPN::TrackerManipulator();
    unsigned int pos = viewer.addCameraManipulator( manip );
    viewer.selectCameraManipulator( pos );

    manip->setTracker( new osgVRPN::Tracker( CAMERA_TRACKER_NAME ) );
    manip->setAutoComputeHomePosition(false);
    manip->setHomeMatrix( osg::Matrix::translate(0,0,10) * osg::Matrixd::rotate(1.2,1,0,0) );
#endif

    viewer.setSceneData( buildScene( &viewer ) );
    viewer.realize();
    while( !viewer.done() )
    {
        viewer.sync();
        viewer.update();
        viewer.frame();
    }
    viewer.sync();
    return 0;
}

// vim: set sw=4 ts=8 et ic ai:
