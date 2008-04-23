/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2006 Robert Osfield 
 *
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * OpenSceneGraph Public License for more details.
*/

/* file:      src/osgPlugins/osgfs/Input.h
 * author:    Mike Weiblen http://mew.cx/
 * copyright: (C) 2007 Michael Weiblen
 * license:   OpenSceneGraph Public License (OSGPL)
 * $Id: Input.h,v 1.1 2007/02/18 22:21:59 mew Exp $
*/

#ifndef OSGFS_INPUT
#define OSGFS_INPUT 1

#include <osg/Image>
#include <osg/Node>
#include <osg/Drawable>
#include <osg/StateAttribute>

#include <osgDB/FieldReaderIterator>
#include <osgDB/ReaderWriter>
#include <osgDB/Input>

#include <map>
#include <string>

namespace plugin_osgfs {

class Input : public osgDB::Input
{
    public:
        Input();
        virtual ~Input();

#if 0
        void setOptions(const ReaderWriter::Options* options) { _options = options; }
        const ReaderWriter::Options* getOptions() const { return _options.get(); }

        virtual osg::Object*         readObjectOfType(const osg::Object& compObj);
        virtual osg::Object*         readObjectOfType(const basic_type_wrapper &btw);

        virtual osg::Object*         readObject();
        virtual osg::Image*          readImage();
        virtual osg::Drawable*       readDrawable();
        virtual osg::StateAttribute* readStateAttribute();
        virtual osg::Uniform*        readUniform();
        virtual osg::Node*           readNode();

        virtual osg::Object*         readObject(const std::string& fileName);
        virtual osg::Image*          readImage(const std::string& fileName);
        virtual osg::Node*           readNode(const std::string& fileName);
        
        virtual osg::Object*         getObjectForUniqueID(const std::string& uniqueID);
        virtual void                 regisiterUniqueIDForObject(const std::string& uniqueID,osg::Object* obj);

    private:
    
        typedef std::map< std::string, osg::ref_ptr<osg::Object> > UniqueIDToObjectMapping;
        UniqueIDToObjectMapping _uniqueIDToObjectMap;
        
        osg::ref_ptr<const ReaderWriter::Options> _options;
#endif

};

}

#endif

// vim: set sw=4 ts=8 et ic ai:
