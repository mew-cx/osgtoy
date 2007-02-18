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

/* file:      src/osgPlugins/osgfs/Output.h
 * author:    Mike Weiblen http://mew.cx/
 * copyright: (C) 2007 Michael Weiblen
 * license:   OpenSceneGraph Public License (OSGPL)
 * $Id: Output.h,v 1.1 2007/02/18 22:21:59 mew Exp $
*/

#ifndef OSGFS_OUTPUT
#define OSGFS_OUTPUT 1

#include <osg/Object>

#include <osgDB/ReaderWriter>
#include <osgDB/Output>

#include <string>
#include <map>
#include <fstream>

namespace plugin_osgfs {

/** ofstream wrapper class for adding support for indenting.
    Used in output of .osg ASCII files to improve their readability.*/
class Output : public osgDB::Output  // formerly public std::ofstream
{
    public:

        Output();
        Output(const char* name);
        
        virtual ~Output();

#if 0
       
        void setOptions(const ReaderWriter::Options* options) { _options = options; }
        const ReaderWriter::Options* getOptions() const { return _options.get(); }

        void open(const char *name);
        
        // comment out temporarily to avoid compilation problems, RO Jan 2002.
        // void open(const char *name,int mode);

        Output& indent();

        /** wrap a string with "" quotes and use \" for any internal quotes.*/
        std::string wrapString(const std::string& str);

        inline void setIndentStep(int step) { _indentStep = step; }
        inline int getIndentStep() const { return _indentStep; }

        inline void setIndent(int indent)  { _indent = indent; }
        inline int getIndent() const { return _indent; }

        inline void setNumIndicesPerLine(int num) { _numIndicesPerLine = num; }
        inline int getNumIndicesPerLine() const { return _numIndicesPerLine; }

        void moveIn();
        void moveOut();
        
        virtual bool writeObject(const osg::Object& obj);
        

        bool getUniqueIDForObject(const osg::Object* obj,std::string& uniqueID);
        bool createUniqueIDForObject(const osg::Object* obj,std::string& uniqueID);
        bool registerUniqueIDForObject(const osg::Object* obj,std::string& uniqueID);

        enum PathNameHint
        {
            AS_IS,
            FULL_PATH,
            RELATIVE_PATH,
            FILENAME_ONLY
        };
        
        inline void setPathNameHint(const PathNameHint pnh) { _pathNameHint = pnh; }
        inline PathNameHint getPathNameHint() const { return _pathNameHint; }

        virtual std::string getFileNameForOutput(const std::string& filename) const;
        const std::string& getFileName() const { return _filename; }

        void setOutputTextureFiles(bool flag) { _outputTextureFiles = flag; }
        bool getOutputTextureFiles() const { return _outputTextureFiles; }

        virtual std::string getTextureFileNameForOutput();
        
    protected:


        virtual void init();

        osg::ref_ptr<const ReaderWriter::Options> _options;

        int _indent;
        int _indentStep;
        
        int _numIndicesPerLine;

        typedef std::map<const osg::Object*,std::string> UniqueIDToLabelMapping;
        UniqueIDToLabelMapping _objectToUniqueIDMap;
        
        std::string _filename;
        
        PathNameHint _pathNameHint;
        bool _outputTextureFiles;
        unsigned int _textureFileNameNumber;
#endif

};

}

#endif

// vim: set sw=4 ts=8 et ic ai:
