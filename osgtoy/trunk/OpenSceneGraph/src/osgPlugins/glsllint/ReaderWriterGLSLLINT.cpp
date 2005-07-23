/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2005 Robert Osfield 
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

/* file:        src/osgPlugins/glsllint/ReaderWriterGLSLLINT.cpp
 * author:      Mike Weiblen http://mew.cx/ 2005-07-21
 * copyright:   (C) 2005 Michael Weiblen
 * license:     OpenSceneGraph Public License (OSGPL)
*/

#include <string>
#include <sstream>

#include <osg/Notify>
#include <osg/Node>
#include <osgDB/ReaderWriter>
#include <osgDB/FileNameUtils>
#include <osgDB/Registry>
#include <osgDB/ReadFile>

#include <osgToy/GlslLint>
#include <osgToy/GlslLintVisitor>

#define EXTENSION_NAME "glsllint"

///////////////////////////////////////////////////////////////////////////

/**
 * An OSG reader plugin for the ".glsllint" pseudo-loader, which runs
 * osgToy::GlslLintVisitor on the loaded model.
 *
 * Usage: <modelfile.ext>.glsllint
 * where:
 *      <modelfile.ext> = an model filename.
 *
 * example: osgviewer glsl_effect.osg.glsllint
 */

class ReaderWriterGLSLLINT : public osgDB::ReaderWriter
{
public:
    ReaderWriterGLSLLINT()
    { }
    
    virtual const char* className() const { return "GlslLintVisitor pseudo-loader"; }

    virtual bool acceptsExtension(const std::string& extension) const
    { 
        return osgDB::equalCaseInsensitive( extension, EXTENSION_NAME );
    }

    virtual ReadResult readNode(const std::string& fileName, const osgDB::ReaderWriter::Options* rw_options) const
    {
        std::string ext = osgDB::getLowerCaseFileExtension(fileName);
        if( !acceptsExtension(ext) )  return ReadResult::FILE_NOT_HANDLED;

        osgToy::GlslLint::Options lint_options = osgToy::GlslLint::NONE;
        if( rw_options )
        {
            std::istringstream iss( rw_options->getOptionString() );
            std::string opt;

            while( getline( iss, opt, ',' ) )
            {
                if( opt == "verbose")  lint_options = osgToy::GlslLint::VERBOSE;
                else
                {
                    osg::notify(osg::WARN)
                        << "\n" EXTENSION_NAME " plugin: unrecognized option \"" << opt << "\"\n"
                        << "comma-delimited options:\n"
                        << "\tverbose = enable verbose output\n"
                        << "example: osgviewer -O verbose foo.osg.glsllint" "\n"
                        << std::endl;
                }
            }
        }

        osg::notify(osg::INFO) << "ReaderWriterGLSLLINT( \"" << fileName << "\" )" << std::endl;

        // strip the pseudo-loader extension
        std::string subFileName = osgDB::getNameLessExtension( fileName );
        if( subFileName == fileName )
        {
            osg::notify(osg::WARN) << "Missing subfilename for " EXTENSION_NAME " pseudo-loader" << std::endl;
            return ReadResult::FILE_NOT_HANDLED;
        }

        // recursively load the subfile.
        osg::Node *node = osgDB::readNodeFile( subFileName );
        if( !node )
        {
            // propagate the read failure upwards
            osg::notify(osg::WARN) << "Subfile \"" << subFileName << "\" could not be loaded" << std::endl;
            return ReadResult::FILE_NOT_HANDLED;
        }


        // This is where the pseudoloader does its thing...

osg::notify(osg::INFO) << "\n\n\n==========================================" << std::endl;
osg::notify(osg::INFO) << "BEGIN GLSLLINT..." << std::endl;

        osgToy::GlslLintVisitor lv( lint_options );
        node->accept( lv );

osg::notify(osg::INFO) << "...END GLSLLINT" << std::endl;
osg::notify(osg::INFO) << "==========================================\n\n\n" << std::endl;

        return node;
    }
};


// Add ourself to the Registry to instantiate the reader/writer.
osgDB::RegisterReaderWriterProxy<ReaderWriterGLSLLINT> g_readerWriter_GLSLLINT_Proxy;

// vim: set sw=4 ts=8 et ic ai:
