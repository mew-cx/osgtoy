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
 * author:    Mike Weiblen http://mew.cx/ 2008-11-06
 * copyright: (C) 2008 Michael Weiblen
 * license:   OpenSceneGraph Public License (OSGPL)
*/

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


        osg::ComputeBoundsVisitor cbbv;
        scene->accept(cbbv);
        const osg::BoundingBox& bbox( cbbv.getBoundingBox() );
    
        osg::notify(osg::NOTICE)
            << "\nBounding box of \"" << subfile << "\":"
            << "\n\tmin: " << bbox._min
            << "\n\tmax: " << bbox._max
            << std::endl;

        osg::Geode* geode( new osg::Geode );
        geode->addDrawable( new osgToy::WirePrismatoid( bbox ) );

        osg::Group* root( new osg::Group );
        root->addChild( scene );
        root->addChild( geode );

        return root;
    }
};


// Add ourself to the Registry to instantiate the reader/writer.
REGISTER_OSGPLUGIN( bbox, ReaderWriterBBOX )



#if 0
// |  usage example:  
// |    CcalculateBoundingBox bbox ;
// |    node->accept( bbox  );
// |    osg::BoundingBox boxExtents = bboxCalc.bbox();
 
#include <osg/NodeVisitor> 
#include <osg/BoundingBox> 
#include <osg/BoundingSphere> 
#include <osg/MatrixTransform> 
#include <osg/Billboard> 
 
 
class  CcalculateBoundingBox : public osg::NodeVisitor { 
 
public: 
 
     CcalculateBoundingBox() : NodeVisitor( NodeVisitor::TRAVERSE_ALL_CHILDREN ) {
            m_transformMatrix.makeIdentity();
     }

    virtual ~CcalculateBoundingBox() {}


    virtual void apply( osg::Geode &geode ) {

        osg::BoundingBox bbox;

        // update bounding box for each drawable
        for(  unsigned int i = 0; i < geode.getNumDrawables(); ++i ){
            // expand the overall bounding box
            bbox.expandBy( geode.getDrawable( i )->getBound());

        }

        // transform corners by current matrix
        osg::BoundingBox bboxTrans;
        for( unsigned int i = 0; i < 8; ++i ) {
            osg::Vec3 xvec = bbox.corner( i ) * m_transformMatrix;
            bboxTrans.expandBy( xvec );
        }

        // update the overall bounding box size
        m_boundingBox.expandBy( bboxTrans );

        // continue traversing through the graph
        traverse( geode );

    }


    virtual void apply( osg::MatrixTransform &node ) {
        m_transformMatrix *= node.getMatrix();
        traverse( node );
    }

    virtual void apply( osg::Billboard &node ){
        // ignore billboard to avoid affecting bounding box.
        traverse( node );
    }

    osg::BoundingBox &getBoundBox() { return m_boundingBox; }  

protected :
    osg::BoundingBox m_boundingBox;          // the overall resultant bounding box
    osg::Matrix      m_transformMatrix;      // the current transform matrix

};

#endif

// vim: set sw=4 ts=8 et ic ai:
