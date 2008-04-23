#include <osg/Vec3>
#include <osg/Vec4>
#include <osg/Quat>
#include <osg/Matrix>
#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/Transform>
#include <osg/NodeCallback>
#include <osg/Depth>
#include <osg/TexMat>
#include <osg/TexGen>
#include <osg/TexEnv>
#include <osg/TextureCubeMap>
#include <osgDB/ReadFile>
#include <osgProducer/Viewer>

///////////////////////////////////////////////////////////////////////////

//#define CUBEMAP_FILENAME(face) "Cubemap_snow/" #face ".jpg"
#define CUBEMAP_FILENAME(face) "Cubemap_test/" #face ".png"

osg::TextureCubeMap* readCubeMap()
{
    osg::TextureCubeMap* cubemap = new osg::TextureCubeMap;

    cubemap->setImage(osg::TextureCubeMap::POSITIVE_X, osgDB::readImageFile(CUBEMAP_FILENAME(posx)));
    cubemap->setImage(osg::TextureCubeMap::NEGATIVE_X, osgDB::readImageFile(CUBEMAP_FILENAME(negx)));
    cubemap->setImage(osg::TextureCubeMap::POSITIVE_Y, osgDB::readImageFile(CUBEMAP_FILENAME(posy)));
    cubemap->setImage(osg::TextureCubeMap::NEGATIVE_Y, osgDB::readImageFile(CUBEMAP_FILENAME(negy)));
    cubemap->setImage(osg::TextureCubeMap::POSITIVE_Z, osgDB::readImageFile(CUBEMAP_FILENAME(posz)));
    cubemap->setImage(osg::TextureCubeMap::NEGATIVE_Z, osgDB::readImageFile(CUBEMAP_FILENAME(negz)));

    cubemap->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
    cubemap->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
    cubemap->setWrap(osg::Texture::WRAP_R, osg::Texture::CLAMP_TO_EDGE);

    cubemap->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR);
    cubemap->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);

    return cubemap;
}

///////////////////////////////////////////////////////////////////////////

// Update texture matrix for cubemaps
struct TexMatCallback : public osg::NodeCallback
{
public:
    TexMatCallback(osg::TexMat& tm) : _texMat(tm) {}

    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
    {
        osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(nv);
        if( cv )
        {
            const osg::Matrix& MV = cv->getModelViewMatrix();
            const osg::Matrix R = osg::Matrix::rotate( osg::DegreesToRadians(112.0f), 0.0f,0.0f,1.0f)*
                                  osg::Matrix::rotate( osg::DegreesToRadians(90.0f), 1.0f,0.0f,0.0f);

            osg::Quat q;
            MV.get( q );
            const osg::Matrix C = osg::Matrix::rotate( q.inverse() );
            _texMat.setMatrix( C * R );
        }
        traverse(node,nv);
    }

    osg::TexMat& _texMat;
};

///////////////////////////////////////////////////////////////////////////

class MoveEarthySkyWithEyePointTransform : public osg::Transform
{
public:
    /** Get the transformation matrix which moves from local coords to world coords.*/
    virtual bool computeLocalToWorldMatrix(osg::Matrix& matrix,osg::NodeVisitor* nv) const
    {
        osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(nv);
        if (cv)
        {
            osg::Vec3 eyePointLocal = cv->getEyeLocal();
            matrix.preMult(osg::Matrix::translate(eyePointLocal));
        }
        return true;
    }

    /** Get the transformation matrix which moves from world coords to local coords.*/
    virtual bool computeWorldToLocalMatrix(osg::Matrix& matrix,osg::NodeVisitor* nv) const
    {
        osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(nv);
        if (cv)
        {
            osg::Vec3 eyePointLocal = cv->getEyeLocal();
            matrix.postMult(osg::Matrix::translate(-eyePointLocal));
        }
        return true;
    }
};

///////////////////////////////////////////////////////////////////////////

osg::Node* createSkyBox()
{

    osg::StateSet* stateset = new osg::StateSet();

    osg::TexEnv* te = new osg::TexEnv;
    te->setMode(osg::TexEnv::REPLACE);
    stateset->setTextureAttributeAndModes(0, te, osg::StateAttribute::ON);

    osg::TexGen *tg = new osg::TexGen;
    tg->setMode(osg::TexGen::NORMAL_MAP);
    stateset->setTextureAttributeAndModes(0, tg, osg::StateAttribute::ON);

    osg::TexMat *tm = new osg::TexMat;
    stateset->setTextureAttribute(0, tm);

    osg::TextureCubeMap* skymap = readCubeMap();
    stateset->setTextureAttributeAndModes(0, skymap, osg::StateAttribute::ON);

    stateset->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
    stateset->setMode( GL_CULL_FACE, osg::StateAttribute::OFF );

    // clear the depth to the far plane.
    osg::Depth* depth = new osg::Depth;
    depth->setFunction( osg::Depth::ALWAYS );
    depth->setRange( 1.0, 1.0 );
    stateset->setAttributeAndModes( depth, osg::StateAttribute::ON );

    stateset->setRenderBinDetails( -1, "RenderBin" );

    osg::Geode* geode = new osg::Geode;
    geode->setCullingActive(false);
    geode->setStateSet( stateset );
    geode->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0,0,0),1)));

    osg::Transform* transform = new MoveEarthySkyWithEyePointTransform;
    transform->setCullingActive(false);
    transform->addChild(geode);

    osg::ClearNode* clearNode = new osg::ClearNode;
//  clearNode->setRequiresClear(false);
    clearNode->setCullCallback(new TexMatCallback(*tm));
    clearNode->addChild(transform);

    return clearNode;
}

///////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
    osg::ArgumentParser arguments(&argc,argv);

    arguments.getApplicationUsage()->setDescription(arguments.getApplicationName()+" is example for skybox");
    arguments.getApplicationUsage()->setCommandLineUsage(arguments.getApplicationName()+" [options] filename ...");
    arguments.getApplicationUsage()->addCommandLineOption("-h or --help","Display this information");

    osgProducer::Viewer viewer(arguments);
    viewer.setUpViewer(osgProducer::Viewer::STANDARD_SETTINGS);

    viewer.getUsage(*arguments.getApplicationUsage());
    if (arguments.read("-h") || arguments.read("--help"))
    {
        arguments.getApplicationUsage()->write(std::cout);
        return 1;
    }

    arguments.reportRemainingOptionsAsUnrecognized();
    if (arguments.errors())
    {
        arguments.writeErrorMessages(std::cout);
        return 1;
    }

    osg::Group* scene = new osg::Group;

    scene->addChild( createSkyBox() );

    osg::Node* model = osgDB::readNodeFiles( arguments );
    if( !model )
    {
        osg::Geode* geode = new osg::Geode;
        geode->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0,0,0),1)));
        model = geode;
    }
    scene->addChild( model );

    viewer.setSceneData( scene );
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
