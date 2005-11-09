/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2004 Robert Osfield 
 *
 * This application is open source and may be redistributed and/or modified   
 * freely and without restriction, both in commericial and non commericial applications,
 * as long as this copyright notice is maintained.
 * 
 * This application is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

/* file:	examples/osgVRPNviewer/osgVRPNviewer.cpp
 * author:	Mike Weiblen mew@mew.cx 2004-08-14
 * copyright:	(C) 2003-2004 Michael Weiblen
 * license:	OpenSceneGraph Public License (OSGPL)
 *
 * references:	http://www.openscenegraph.org/
 *		http://www.vrpn.org/
 *		http://www.mew.cx/osg/
 *
 * depends:	osgVRPN 0.2.1, OSG 0.9.7, VRPN 06.04
*/

#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/Group>
#include <osg/ref_ptr>
#include <osgDB/ReadFile>
#include <osgProducer/Viewer>

#define USE_TRACKER
//#define TRACKER_ON_NODE

#ifdef USE_TRACKER //[
#include "osgVRPN/Tracker.h"
#include "osgVRPN/TrackerTransform.h"
#include "osgVRPN/TrackerManipulator.h"
#endif //]

///////////////////////////////////////////////////////////////////////////

int main( int argc, char **argv )
{
    // use an ArgumentParser object to manage the program arguments.
    osg::ArgumentParser arguments(&argc,argv);

    // set up the usage document, in case we need to print out how to use this program.
    arguments.getApplicationUsage()->setApplicationName(arguments.getApplicationName());
    arguments.getApplicationUsage()->setDescription(arguments.getApplicationName()+" demonstrates osgVRPN");
    arguments.getApplicationUsage()->setCommandLineUsage(arguments.getApplicationName()+" [options]");
    arguments.getApplicationUsage()->addCommandLineOption("-h or --help","Display this information");

    // construct the viewer.
    osgProducer::Viewer viewer(arguments);
    viewer.setClearColor( osg::Vec4( 0.5, 0.2, 0.2, 1.0 ) );

    // set up the value with sensible default event handlers.
    viewer.setUpViewer(osgProducer::Viewer::STANDARD_SETTINGS);

#ifdef USE_TRACKER //[
    osgVRPN::TrackerManipulator* manip = new osgVRPN::TrackerManipulator();
    unsigned int pos = viewer.addCameraManipulator( manip );
    viewer.selectCameraManipulator( pos );
#endif //]

    // get details on keyboard and mouse bindings used by the viewer.
    viewer.getUsage(*arguments.getApplicationUsage());

    // if user request help write it out to cout.
    if (arguments.read("-h") || arguments.read("--help"))
    {
        arguments.getApplicationUsage()->write(std::cout);
        return 1;
    }

#ifdef USE_TRACKER //[
    osgVRPN::TrackerTransform* xform = new osgVRPN::TrackerTransform;
#endif //]

    osg::ref_ptr<osg::Group> group = new osg::Group;
    osg::Geode* geode = new osg::Geode;
    geode->addDrawable(new osg::ShapeDrawable(new osg::Cone( osg::Vec3(0,0,0), 0.5f, 1.0f )));
    xform->addChild( geode );
    group->addChild( xform );

    geode = new osg::Geode;
    geode->addDrawable(new osg::ShapeDrawable(new osg::Box( osg::Vec3(0,0,0), 3.0f, 3.0f, 0.05f )));
    group->addChild( geode );

    viewer.setSceneData( group.get() );
    viewer.realize();

#ifdef USE_TRACKER //[
    osg::ref_ptr<osgVRPN::Tracker> gTracker = new osgVRPN::Tracker( "Tracker0@localhost" );
#ifdef TRACKER_ON_NODE
    // attach tracker to cone's transform.
    xform->setTracker( gTracker );
#else
    // attach tracker to camera's manipulator.
    manip->setTracker( gTracker );
#endif
#endif //]

    while( !viewer.done() )
    {
        viewer.sync();
        viewer.update();
        viewer.frame();
    }

    viewer.sync();
    return 0;
}

/*EOF*/
