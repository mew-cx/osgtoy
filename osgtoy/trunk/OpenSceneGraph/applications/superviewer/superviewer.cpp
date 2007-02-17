/* file:        applications/superviewer/superviewer.cpp
 * author:      Mike Weiblen
 * copyright:   (C) 2005-2007 Michael Weiblen  http://mew.cx/
 * license:     OpenSceneGraph Public License (OSGPL)
 * $Id: superviewer.cpp,v 1.1 2007/02/17 05:00:57 mew Exp $
*/

/* TODOs
- keypress to reload cmdline scene from files (in case they changed)
- keypress to save scene to .ive
- keypress to save screengrab to .rgb or .png (will that preserve alpha??)
  (see ViewerEventHandler::SnapImageDrawCallback)
- grab/recall saved camera locations
- global Uniforms: 1HzSin, 1HzCos, time that re-zeros from keybd.
- cmdline switch to rotate model for y-up, etc
- osgVRPN-aware: smart camera reference frame for Spaceball/Analog
- network interface w/ lua interpreter, to support remote manip of Uniforms.
- cmdline flag to load cubemap (pattern for 6 images, or 1 cross-cube image)
- load .frag/.vert files from cmdline (or a lua wrapper?)
- optional hud overlays for orientation and map location (you are here)
- how to export camera/viewer info for access by plugins?
- a wandering/orbiting light source
*/

#include <osg/Notify>
#include <osg/ref_ptr>
#include <osg/io_utils>
#include <osg/Geode>
#include <osg/Group>
#include <osg/Node>
#include <osg/Image>
#include <osg/Math>
#include <osg/FrontFace>
#include <osg/MatrixTransform>
#include <osgDB/WriteFile>
#include <osgDB/ReadFile>
#include <osgUtil/Optimizer>
#include <osgText/Text>
#include <osgProducer/Viewer>
#include <osgToy/Polyhedra>
#include <osgToy/LorenzAttractor>
#include <Producer/Camera>

#include "svUtil.h"

///////////////////////////////////////////////////////////////////////////
// from osgmultiplecameras

#define ENABLE_REAR_VIEW 0
#ifdef ENABLE_REAR_VIEW //[
osg::Node* createRearView(osg::Node* subgraph, const osg::Vec4& clearColour)
{
    osg::CameraNode* camera = new osg::CameraNode;
    camera->setViewport( 20, 20, 150, 100 );
    camera->setCullingActive( false );
    camera->setReferenceFrame( osg::Transform::RELATIVE_RF );
    camera->setTransformOrder( osg::CameraNode::POST_MULTIPLY );
    camera->setProjectionMatrix( osg::Matrix::scale(-1,1,1) );
    camera->setViewMatrix( osg::Matrix::rotate(osg::inDegrees(180.f), 0,1,0) );
    camera->setClearColor( osg::Vec4(0.3,0,0,1) );
    camera->setClearMask( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    camera->setRenderOrder( osg::CameraNode::POST_RENDER );
    camera->addChild( subgraph );

    // switch off back face culling as we've swapped over the projection
    // matrix making back faces become front faces.
    camera->getOrCreateStateSet()->setAttribute(new osg::FrontFace(osg::FrontFace::CLOCKWISE));

    return camera;
}
#endif //]

///////////////////////////////////////////////////////////////////////////
// from osghud

#define ENABLE_HUD 0
#ifdef ENABLE_HUD //[
osg::Node* createHUD()
{
    osg::Geode* geode = new osg::Geode();
    osg::StateSet* ss = geode->getOrCreateStateSet();
    ss->setMode( GL_LIGHTING, osg::StateAttribute::OFF );

    std::string fontName( "fonts/arial.ttf" );
    osg::Vec3 position( 150, 800, 0 );
    osg::Vec3 delta( 0, -120, 0 );

    {
        osgText::Text* text = new  osgText::Text;
        text->setText( "HUD text\nfile modified " __TIMESTAMP__ "\nfile compiled " __DATE__ " " __TIME__ );
        text->setFont( fontName );
        text->setColor( osg::Vec4(1,1,0,1) );
        text->setPosition( position );
        position += delta;
        geode->addDrawable( text );
    }

    {
        osg::BoundingBox bb;
        for(unsigned int i=0;i<geode->getNumDrawables();++i)
        {
            bb.expandBy( geode->getDrawable(i)->getBound() );
        }

        osg::Geometry* geom = new osg::Geometry;

        osg::Vec3Array* vertices = new osg::Vec3Array;
        float depth = bb.zMin() - 0.1;                          // in front or in back?
        vertices->push_back( osg::Vec3( bb.xMin(), bb.yMax(), depth ) );
        vertices->push_back( osg::Vec3( bb.xMin(), bb.yMin(), depth ) );
        vertices->push_back( osg::Vec3( bb.xMax(), bb.yMin(), depth ) );
        vertices->push_back( osg::Vec3( bb.xMax(), bb.yMax(), depth ) );
        geom->setVertexArray(vertices);

        osg::Vec3Array* normals = new osg::Vec3Array;
        normals->push_back( osg::Vec3(0,0,1) );
        geom->setNormalArray(normals);
        geom->setNormalBinding(osg::Geometry::BIND_OVERALL);

        osg::Vec4Array* colors = new osg::Vec4Array;
        colors->push_back( osg::Vec4(0.2f,1,0.2f,0.2f) );
        geom->setColorArray(colors);
        geom->setColorBinding(osg::Geometry::BIND_OVERALL);

        geom->addPrimitiveSet(new osg::DrawArrays(GL_QUADS,0,4));

        osg::StateSet* ss = geom->getOrCreateStateSet();
        ss->setMode( GL_BLEND, osg::StateAttribute::ON );
        ss->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );         // drawn last?

        geode->addDrawable( geom );
    }

    osg::CameraNode* camera = new osg::CameraNode;
    camera->setProjectionMatrix( osg::Matrix::ortho2D(0,1280,0,1024) );
    camera->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
    camera->setViewMatrix( osg::Matrix::identity() );
    camera->setClearMask( GL_DEPTH_BUFFER_BIT );
    camera->setRenderOrder( osg::CameraNode::POST_RENDER );
    camera->addChild( geode );

    return camera;
}
#endif //]

///////////////////////////////////////////////////////////////////////////
// from OpenSceneGraph/src/osgProducer/ViewerEventHandler.cpp
// to invoke:
// OsgCameraGroup::getCameraConfig()->getCamera(i)->addPostDrawCallback(snapImageDrawCallback);

class SnapImageDrawCallback : public Producer::Camera::Callback
{
public:
    SnapImageDrawCallback() : _snapImageOnNextFrame(false) {}

    void setFileName(const std::string& filename) { _filename = filename; }
    const std::string& getFileName() const { return _filename; }

    void setSnapImageOnNextFrame(bool flag) { _snapImageOnNextFrame = flag; }
    bool getSnapImageOnNextFrame() const { return _snapImageOnNextFrame; }

    virtual void operator()( const Producer::Camera &camera )
    {
        if( !_snapImageOnNextFrame ) return;

        int x, y;
        unsigned int w, h;
        camera.getProjectionRectangle(x,y,w,h);

        osg::ref_ptr<osg::Image> image = new osg::Image;
        //NEEDED?  image->allocateImage( w, h, 1, GL_RGB, GL_UNSIGNED_BYTE);
        image->readPixels( x, y, w, h, GL_RGB, GL_UNSIGNED_BYTE );

        osgDB::writeImageFile( *image, _filename );
        _snapImageOnNextFrame = false;

        //char filename[128];
        //sprintf( filename, "grab%04d.bmp", _screenCaptureSequence);
        //osgDB::writeImageFile( *(image.get()), filename );
        //_screenCaptureSequence++;
    }

protected:
    std::string _filename;
    bool        _snapImageOnNextFrame;
};


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

static osg::Group* createScene( osgProducer::Viewer& viewer, osg::ArgumentParser& args )
{
    osg::Group* scene = new osg::Group;
    osg::Node* node = 0;


#if 0 //[
    // load model file(s) from cmdline args
    osg::Timer_t t0 = osg::Timer::instance()->tick();
    node = osgDB::readNodeFiles( args );
    if( !node ) node = osgDB::readNodeFile( "cow.osg" );
    osg::Timer_t t1 = osg::Timer::instance()->tick();
    std::cout << "Time to load = " << osg::Timer::instance()->delta_s(t0,t1) << std::endl;
    scene->addChild( node );
#endif //]


#if 0 //[
{
    osg::Geode* geode = new osg::Geode;

    geode->addDrawable( new osgToy::PenroseTriangle );
    //geode->addDrawable( new osgToy::RhombicDodecahedron );
    //geode->addDrawable( new osgToy::ColorOriginTetra );
    //geode->addDrawable( new osgToy::OctoStrip );
    //geode->addDrawable( new osgToy::TetraStrip );

    scene->addChild( geode );
}
#endif //]


#if 0 //[
osg::Node* makeOctoSphere( unsigned int level );
    scene->addChild( makeOctoSphere(5) );
#endif //]


#if 0 //[
    osg::Geode* geode = new osg::Geode;
    geode->addDrawable( new osgToy::LorenzAttractor(40000) );
    scene->addChild(geode);
#endif //]


#if 0 //[
// this is basically glsl_simple.osg
osg::Node* makeScene1();
    scene->addChild( makeScene1() );
#endif //]


#if 0 //[
// a denser version of glsl_confetti.osg
osg::Node* makeConfetti( int count );
    scene->addChild( makeConfetti( 100 ) );
    // TEMPORARY workaround for lack of bounding volume specification
    viewer.getCullSettings().setComputeNearFarMode( osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR );
#endif //]


#if 0 //[
osg::Node* makeCompactDiscScene();
    scene->addChild( makeCompactDiscScene() );
#endif //]


#if 1 //[
osg::Node* makeSectorStarScene();
    scene->addChild( makeSectorStarScene() );
#endif //]


#if 0 //[
osg::Node* makeDataflagScene3();
    scene->addChild( makeDataflagScene3() );
#endif //]


#if 0 //[
osg::Node* makeMipmapScene();
    scene->addChild( makeMipmapScene() );
#endif //]


#if 0 //[
// from TexTest.cpp & StaticImage.cpp:
// experiment of defining texels with C++ rather than loading
// an image file.  not working yet.
osg::Node* makeTexTestScene();
    scene->addChild( makeTexTestScene() );
#endif //]


#if 0 //[
// a work-in-progress (not even begun really)
osg::Node* makeJack();
    scene->addChild( makeJack() );
#endif //]


#if 0 //[
osg::Node* makeSphereGrid( int width, int height, int depth );
    scene->addChild( makeSphereGrid( 4, 3, 2 ) );
#endif //]


#if 0 //[
osg::Node* makeShadedSpheres();
    scene->addChild( makeShadedSpheres() );
#endif //]


#if 0 //[
    // scale the scene down by 100x (for shrinking xyz100.osg)
    const float s = 0.01f;
    osg::MatrixTransform *xform = new osg::MatrixTransform;
    xform->setDataVariance( osg::Object::STATIC );
    xform->setMatrix( osg::Matrix::scale( s, s, s ) );
    xform->addChild( scene );
    scene = xform;

    osg::StateSet* ss = scene->getOrCreateStateSet();
    ss->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
#endif //]

    return scene;
}

///////////////////////////////////////////////////////////////////////////

static void DirectionColor( osgProducer::Viewer& viewer )
{
    osg::Vec3 eye, center, up;
    viewer.getViewMatrix().getLookAt( eye, center, up );
    osg::Vec3 dirColor( center - eye );

    float scale = 0.0f;
    scale = osg::maximum( osg::absolute( dirColor.x() ), scale );
    scale = osg::maximum( osg::absolute( dirColor.y() ), scale );
    scale = osg::maximum( osg::absolute( dirColor.z() ), scale );
    dirColor /= scale;
    //dirColor *= 0.5f;
    //dirColor += osg::Vec3( 0.5, 0.5, 0.5 );

    //osg::notify(osg::INFO) << dirColor << std::endl;

    viewer.setClearColor( osg::Vec4(dirColor,1) );
}

///////////////////////////////////////////////////////////////////////////

int main( int argc, char **argv )
{
    osg::ArgumentParser args( &argc, argv );
    args.getApplicationUsage()->setApplicationName(args.getApplicationName());
    args.getApplicationUsage()->setDescription(args.getApplicationName()+" is mew's testbench");
    args.getApplicationUsage()->setCommandLineUsage(args.getApplicationName()+" [options] filename ...");
    args.getApplicationUsage()->addCommandLineOption("-h","display all help.");

    args.getApplicationUsage()->addKeyboardMouseBinding( "x", "Reload and recompile shader source files." );

    osgProducer::Viewer viewer( args );
    viewer.setUpViewer( osgProducer::Viewer::TRACKBALL_MANIPULATOR |
            osgProducer::Viewer::STATE_MANIPULATOR  |
            osgProducer::Viewer::HEAD_LIGHT_SOURCE  |
            //osgProducer::Viewer::SKY_LIGHT_SOURCE   |
            osgProducer::Viewer::VIEWER_MANIPULATOR |
            osgProducer::Viewer::ESCAPE_SETS_DONE );

    viewer.getUsage( *args.getApplicationUsage() );

    if( args.read("-h") )
    {
        args.getApplicationUsage()->write( std::cout,
                osg::ApplicationUsage::COMMAND_LINE_OPTION |
                osg::ApplicationUsage::ENVIRONMENTAL_VARIABLE |
                osg::ApplicationUsage::KEYBOARD_MOUSE_BINDING );
        return 1;
    }

    if( args.errors() )
    {
        args.writeErrorMessages(std::cout);
        return 1;
    }

    osg::Group* scene = createScene( viewer, args );

    args.reportRemainingOptionsAsUnrecognized();
    if( args.errors() )
    {
        args.writeErrorMessages(std::cout);
        return 1;
    }

    viewer.getEventHandlerList().push_front( new svUtil::EventHandler );

#ifdef ENABLE_REAR_VIEW //[
    osg::Group* group = new osg::Group;
    group->addChild( scene );
    group->addChild( createRearView( scene, viewer.getClearColor() ) );
    scene = group;
#endif //]

#ifdef ENABLE_HUD //[
    scene->addChild( createHUD() );
#endif //]

    //osgUtil::Optimizer optimizer;
    //optimizer.optimize( scene );

#if 0 //[
void addVrpnManip( osgProducer::Viewer& viewer );
    addVrpnManip( viewer );
#endif //]

    viewer.setSceneData( scene );
    viewer.realize();
    while( !viewer.done() )
    {
        viewer.sync();
        viewer.update();
        DirectionColor( viewer );
        viewer.frame();
    }
    viewer.sync();
    return 0;
}

// vim: set sw=4 ts=8 et ic ai:
