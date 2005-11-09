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
 * $Id: osgVRPNviewer.cpp,v 1.4 2005/11/09 08:25:36 mew Exp $
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


// enable this macro to attach a Tracker to the camera's manipulator
#define CAMERA_TRACKER_NAME "Tracker0@localhost"

// enable this macro to attach a Tracker to a scenegraph transform node
//#define NODE_TRACKER_NAME "Tracker0@localhost"

///////////////////////////////////////////////////////////////////////////

class EventHandler : public osgGA::GUIEventHandler
{
public:
    EventHandler( osg::ref_ptr<osgVRPN::Tracker> tracker ) : _tracker( tracker ) {}

    bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
    {
        if( ea.getEventType() != osgGA::GUIEventAdapter::KEYDOWN ) return false;

        switch( ea.getKey() )
        {
            case osgGA::GUIEventAdapter::KEY_Up:
                adjustScale( 1.1f );
                break;

            case osgGA::GUIEventAdapter::KEY_Down:
                adjustScale( 1.0f / 1.1f );
                break;

            default: break;
        }

        return false;
    }

private:
    void adjustScale( float multiplier )
    {
        if( ! _tracker.valid() ) return;
        float scale = _tracker->getScale() * multiplier;
        _tracker->setScale( scale );
        osg::notify(osg::INFO) << "Tracker scale = " << scale << std::endl;
    }

    osg::ref_ptr<osgVRPN::Tracker> _tracker;
};

///////////////////////////////////////////////////////////////////////////

osg::Node* buildScene( osgProducer::Viewer* viewer )
{
    osg::Group* scene = new osg::Group;

    osg::Geode* geode = new osg::Geode;
    geode->addDrawable(new osg::ShapeDrawable(new osg::Cone(osg::Vec3(0,0,0),1,2)));
#if defined(NODE_TRACKER_NAME)
    osgVRPN::TrackerTransform* trkXform = new osgVRPN::TrackerTransform;
    trkXform->setTracker( new osgVRPN::Tracker( NODE_TRACKER_NAME ) );
    trkXform->addChild( geode );
    scene->addChild( trkXform );
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

///////////////////////////////////////////////////////////////////////////

int main( int argc, char **argv )
{
    osg::ArgumentParser args( &argc, argv );
    args.getApplicationUsage()->setApplicationName( args.getApplicationName() );
    args.getApplicationUsage()->setDescription(args.getApplicationName()+" demonstrates osgVRPN trackers");
    args.getApplicationUsage()->setCommandLineUsage(args.getApplicationName()+" [options]");
    args.getApplicationUsage()->addCommandLineOption("-h or --help","Display this information");
    args.getApplicationUsage()->addKeyboardMouseBinding( "upArrow", "increase tracker scale" );
    args.getApplicationUsage()->addKeyboardMouseBinding( "downArrow", "decrease tracker scale" );

    osgProducer::Viewer viewer(args);
    viewer.setUpViewer( osgProducer::Viewer::STANDARD_SETTINGS );

    viewer.getUsage( *args.getApplicationUsage() );
    if (args.read("-h") || args.read("--help"))
    {
        args.getApplicationUsage()->write(std::cout);
        return 1;
    }

#if defined(CAMERA_TRACKER_NAME)
    osgVRPN::TrackerManipulator* trkManip = new osgVRPN::TrackerManipulator();
    unsigned int pos = viewer.addCameraManipulator( trkManip );
    viewer.selectCameraManipulator( pos );

    osg::ref_ptr<osgVRPN::Tracker> tracker = new osgVRPN::Tracker( CAMERA_TRACKER_NAME );
    trkManip->setTracker( tracker );
    trkManip->setAutoComputeHomePosition(false);
    trkManip->setHomeMatrix( osg::Matrix::translate(0,0,10) * osg::Matrixd::rotate(1.2,1,0,0) );

    viewer.getEventHandlerList().push_front( new EventHandler( tracker ) );
#endif

    viewer.setSceneData( buildScene( &viewer ) );
    viewer.realize();
    while( !viewer.done() )
    {
        viewer.sync();
        viewer.update();

        {   // handy diagnostic: set clearcolor based on view orientation
            osg::Vec3 eye, center, up;
            viewer.getViewMatrix().getLookAt( eye, center, up );
            osg::Vec3 dirColor( center - eye );
            dirColor.normalize();
            dirColor += osg::Vec3(1,1,1);
            dirColor *= 0.5f;
            viewer.setClearColor( osg::Vec4(dirColor,1) );
        }

        viewer.frame();
    }
    viewer.sync();
    return 0;
}

// vim: set sw=4 ts=8 et ic ai:
