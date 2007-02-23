/* file:      applications/osgimgconv/osgimgconv.cpp
 * author:    Mike Weiblen http://mew.cx/
 * copyright: (C) 2007 Michael Weiblen
 * license:   OpenSceneGraph Public License (OSGPL)
 * $Id 2007-02-03 $
*/

#include <osg/ArgumentParser>
#include <osg/ApplicationUsage>
#include <osg/Notify>
#include <osg/Image>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <iostream>

int main( int argc, char *argv[] )
{
    osg::ArgumentParser args( &argc, argv );
    args.getApplicationUsage()->setApplicationName(args.getApplicationName());
    args.getApplicationUsage()->setDescription(args.getApplicationName()+" is a utility for converting image formats");
    args.getApplicationUsage()->setCommandLineUsage(args.getApplicationName()+" fileNameIn fileNameOut");

    if( args.argc() != 3 )
    {
        args.getApplicationUsage()->write(std::cout,osg::ApplicationUsage::COMMAND_LINE_OPTION);
        return 1;
    }

    const std::string fileNameIn( args[1] );
    const std::string fileNameOut( args[2] );

    osg::ref_ptr<osg::Image> img( osgDB::readImageFile( fileNameIn ) );
    if( !img.valid() )
    {
        osg::notify(osg::WARN) << "cant read file " << fileNameIn << std::endl;
        return 1;
    }

    img->setName( fileNameIn );
    if( !osgDB::writeImageFile( *img, fileNameOut ) )
    {
        osg::notify(osg::WARN) << "cant write file " << fileNameOut << std::endl;
        return 2;
    }

    osg::notify(osg::NOTICE) << "wrote file " << fileNameOut << std::endl;
    return 0;
}

// vim: set sw=4 ts=8 et ic ai:
