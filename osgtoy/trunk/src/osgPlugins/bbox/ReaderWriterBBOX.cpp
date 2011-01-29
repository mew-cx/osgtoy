/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2004 Robert Osfield 
 *
 * This application is open source and may be redistributed and/or modified   
 * freely and without restriction, both in commericial and non commericial
 * applications, as long as this copyright notice is maintained.
 * 
 * This application is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
*/

/* file:      src/osgPlugins/bbox/ReaderWriterBBOX.cpp
 * author:    Mike Weiblen http://mew.cx/ 2011-01-26
 * copyright: (C) 2011 Michael Weiblen
 * license:   OpenSceneGraph Public License (OSGPL)
*/

#include <osg/NodeVisitor>
#include <osg/BoundingBox>
#include <osg/Polytope>
#include <osg/Transform>
#include <osg/Drawable>
#include <osg/MatrixTransform>
#include <osg/Billboard>

#include <osg/Geode>
#include <osg/Notify>
#include <osg/ComputeBoundsVisitor>
#include <osg/io_utils>

#include <osgDB/ReaderWriter>
#include <osgDB/FileNameUtils>
#include <osgDB/Registry>
#include <osgDB/ReadFile>

#include <osgToy/Geometry.h>

#include <stdio.h>



/////////////////////////////////////////////////////////////////////////////
// this derived from osg::ComputeBoundsVisitor

#if 0

namespace {

class ComputeBoundsVisitor : public osg::NodeVisitor
{
public:
    ComputeBoundsVisitor( osg::NodeVisitor::TraversalMode tm = TRAVERSE_ALL_CHILDREN )
        : NodeVisitor(tm)
    {
        osg::Matrix identity;
        _matrixStack.push_back( identity );
    }

    const osg::BoundingBox& getBoundingBox() const { return _bbox; }

    void getPolytope( osg::Polytope& polytope, float margin=0.1f ) const
    {
        const float delta( _bbox.radius() * margin );
        polytope.add( osg::Plane(  0,  0,  1, delta - _bbox.zMin() ) );
        polytope.add( osg::Plane(  0,  0, -1, delta + _bbox.zMax() ) );

        polytope.add( osg::Plane(  1,  0,  0, delta - _bbox.xMin() ) );
        polytope.add( osg::Plane( -1,  0,  0, delta + _bbox.xMax() ) );

        polytope.add( osg::Plane(  0,  1,  0, delta - _bbox.yMin() ) );
        polytope.add( osg::Plane(  0, -1,  0, delta + _bbox.yMax() ) );
    }
        
    void getBase( osg::Polytope& polytope, float margin=0.1f ) const
    {
        const float delta( _bbox.radius() * margin );
        polytope.add( osg::Plane(  0,  0,  1, delta - _bbox.zMin() ) );
    }

    void apply( osg::Node& node )
    {
        traverse( node );
    }

    void apply( osg::Transform& transform )
    {
        osg::Matrix matrix( _matrixStack.back() );
        transform.computeLocalToWorldMatrix( matrix, this );
        _matrixStack.push_back( matrix );
        traverse( transform );
        _matrixStack.pop_back();
    }
    
    void apply( osg::Geode& geode )
    {
        const unsigned int num( geode.getNumDrawables() );
        for( unsigned int i = 0; i < num; ++i )
        {
            applyDrawable(geode.getDrawable(i));
        }
    }

    void applyDrawable( osg::Drawable* drawable )
    {
        osg::Matrix& matrix( _matrixStack.back() );
        const osg::BoundingBox& dbb( drawable->getBound() );
        if( dbb.valid() )
        {
            _bbox.expandBy( dbb.corner(0) * matrix );
            _bbox.expandBy( dbb.corner(1) * matrix );
            _bbox.expandBy( dbb.corner(2) * matrix );
            _bbox.expandBy( dbb.corner(3) * matrix );
            _bbox.expandBy( dbb.corner(4) * matrix );
            _bbox.expandBy( dbb.corner(5) * matrix );
            _bbox.expandBy( dbb.corner(6) * matrix );
            _bbox.expandBy( dbb.corner(7) * matrix );
        }
    }
    
protected:
    typedef std::vector<osg::Matrix> MatrixStack;
    MatrixStack         _matrixStack;
    osg::BoundingBox    _bbox;
};

}

#endif

/////////////////////////////////////////////////////////////////////////////


namespace {

class BoundingBoxVisitor : public osg::NodeVisitor
{
public:
    BoundingBoxVisitor( osg::NodeVisitor::TraversalMode tm = TRAVERSE_ALL_CHILDREN )
        : NodeVisitor(tm)
    {
    }

    virtual void apply( osg::Geode& geode )
    {
        osg::BoundingBox bb1;
        const unsigned int num( geode.getNumDrawables() );
        for( unsigned int i = 0; i < num; ++i )
        {
            // expand bb1 for each drawable
            bb1.expandBy( geode.getDrawable( i )->getBound());
        }

        // transform geode's bbox by current matrix
        osg::BoundingBox bb2;
        for( unsigned int i = 0; i < 8; ++i )
        {
            osg::Vec3 v( bb1.corner(i) * _xformMatrix );
            bb2.expandBy( v );
        }

        _bbox.expandBy( bb2 );
        traverse( geode );
    }

    virtual void apply( osg::MatrixTransform& node )
    {
        // does this properly pop off old matrices???
        _xformMatrix *= node.getMatrix();
        traverse( node );
    }

    virtual void apply( osg::Billboard& node )
    {
        // ignore billboard
        traverse( node );
    }

    const osg::BoundingBox& getBoundingBox() const { return _bbox; }

protected :
    osg::BoundingBox _bbox;             // result
    osg::Matrix      _xformMatrix;      // current transform matrix
};

}


/////////////////////////////////////////////////////////////////////////////


#define EXTENSION_NAME "bbox"


class ReaderWriterBBOX : public osgDB::ReaderWriter
{
public:
    ReaderWriterBBOX() { }
    
    virtual const char* className() const { return EXTENSION_NAME " pseudo-loader"; }

    virtual bool acceptsExtension( const std::string& extension ) const
    { 
        return osgDB::equalCaseInsensitive( extension, EXTENSION_NAME );
    }

    void usage() const
    {
        osg::notify(osg::NOTICE)
            << "\nUsage for " << className() << ":\n"
            << "<subfile>." EXTENSION_NAME "\n\tdecorate scene with wireframe bounding box\n"
            << std::endl;
    }

    virtual ReadResult readNode( const std::string& fileName, const osgDB::ReaderWriter::Options* options ) const
    {
        std::string ext( osgDB::getLowerCaseFileExtension(fileName) );
        if( ! acceptsExtension(ext) )
            return ReadResult::FILE_NOT_HANDLED;

        osg::notify(osg::INFO) << "ReaderWriterBBOX( \"" << fileName << "\" )" << std::endl;

        // remove the pseudo-loader extension
        std::string subfile( osgDB::getNameLessExtension( fileName ) );
        if( subfile.empty() )
        {
            osg::notify(osg::WARN) << "Missing subfile for " << className() << std::endl;
            return ReadResult::FILE_NOT_HANDLED;
        }

        // recursively load the subfile.
        osg::Node* scene( osgDB::readNodeFile( subfile, options ) );
        if( !scene )
        {
            // propagate the read failure upwards
            osg::notify(osg::WARN) << "Subfile \"" << subfile << "\" could not be loaded" << std::endl;
            return ReadResult::FILE_NOT_HANDLED;
        }


        //osg::ComputeBoundsVisitor bbv;
        //ComputeBoundsVisitor bbv;
        BoundingBoxVisitor bbv;

        scene->accept( bbv );
        const osg::BoundingBox& bbox( bbv.getBoundingBox() );
    
        osg::notify(osg::NOTICE)
            << "Bounding box of \"" << subfile << "\":\n"
            << "\tmin: " << bbox._min << "\n"
            << "\tmax: " << bbox._max << "\n"
            << std::endl;

        osg::Geode* wireBox( new osg::Geode );
        wireBox->addDrawable( new osgToy::WireBox( bbox ) );

        osg::Group* root( new osg::Group );
        root->addChild( scene );
        root->addChild( wireBox );

        return root;
    }
};


// Add ourself to the Registry to instantiate the reader/writer.
REGISTER_OSGPLUGIN( bbox, ReaderWriterBBOX )

// vim: set sw=4 ts=8 et ic ai:
