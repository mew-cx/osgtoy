/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2006 Robert Osfield 
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

/* file:      src/osgPlugins/osgfs/ReaderWriterOSGFS.cpp
 * author:    Mike Weiblen http://mew.cx/
 * copyright: (C) 2007 Michael Weiblen
 * license:   OpenSceneGraph Public License (OSGPL)
 * $Id: ReaderWriterOSGFS.cpp,v 1.1 2007/02/18 22:21:59 mew Exp $
*/

#include <sstream>

#include <osg/Image>
#include <osg/Group>
#include <osg/Notify>

#include <osgDB/FileNameUtils>
#include <osgDB/FileUtils>
#include <osgDB/Registry>

#include "Input.h"
#include "Output.h"

using namespace osg;
using namespace osgDB;

class OSGFSReaderWriter : public ReaderWriter
{
    public:
        virtual const char* className() const { return "OSGFS Reader/Writer"; }

        virtual bool acceptsExtension(const std::string& extension) const
        {
            return equalCaseInsensitive(extension,"osgfs");
        }

        virtual ReadResult readObject(const std::string& fileName, const Options* opt) const { return readNode(fileName, opt); }

        virtual ReadResult readObject(std::istream& fin, const Options* opt) const { return readNode(fin, opt); }

        virtual ReadResult readNode(const std::string& file, const Options* opt) const
        {
            std::string ext = osgDB::getLowerCaseFileExtension(file);
            if (!acceptsExtension(ext)) return ReadResult::FILE_NOT_HANDLED;

            std::string fileName = osgDB::findDataFile( file, opt );
            if (fileName.empty()) return ReadResult::FILE_NOT_FOUND;

            // code for setting up the database path so that internally referenced file are searched for on relative paths. 
            osg::ref_ptr<Options> local_opt = opt ? static_cast<Options*>(opt->clone(osg::CopyOp::SHALLOW_COPY)) : new Options;
            local_opt->setDatabasePath(osgDB::getFilePath(fileName));

            std::ifstream fin(fileName.c_str());
            if (fin)
            {
                return readNode(fin, local_opt.get());
            }
            return 0L;
                        
        }
        
        virtual ReadResult readNode(std::istream& fin, const Options* options) const
        {

            plugin_osgfs::Input fr;
            fr.attach(&fin);
            fr.setOptions(options);
            
            typedef std::vector<osg::Node*> NodeList;
            NodeList nodeList;

            // load all nodes in file, placing them in a group.
            while(!fr.eof())
            {
                Node *node = fr.readNode();
                if (node) nodeList.push_back(node);
                else fr.advanceOverCurrentFieldOrBlock();
            }

            if  (nodeList.empty())
            {
                return ReadResult("No data loaded");
            }
            else if (nodeList.size()==1)
            {
                return nodeList.front();
            }
            else
            {
                Group* group = new Group;
                group->setName("import group");
                for(NodeList::iterator itr=nodeList.begin();
                    itr!=nodeList.end();
                    ++itr)
                {
                    group->addChild(*itr);
                }
                return group;
            }

        }

        void setPrecision(osgDB::Output& fout, const osgDB::ReaderWriter::Options* options) const
        {
            if (options)
            {
                std::istringstream iss(options->getOptionString());
                std::string opt;
                while (iss >> opt)
                {
                    if(opt=="PRECISION" || opt=="precision") 
                    {
                        int prec;
                        iss >> prec;
                        fout.precision(prec);
                    }
                    if (opt=="OutputTextureFiles")
                    {
                        fout.setOutputTextureFiles(false);
                    }
                }
            }
        }            

        virtual WriteResult writeObject(const Object& obj,const std::string& fileName, const osgDB::ReaderWriter::Options* options) const
        {
            std::string ext = osgDB::getLowerCaseFileExtension(fileName);
            if (!acceptsExtension(ext)) return WriteResult::FILE_NOT_HANDLED;

            plugin_osgfs::Output fout(fileName.c_str());
            fout.setOptions(options);
            if (fout)
            {
                setPrecision(fout,options);

                fout.writeObject(obj);
                fout.close();
                return WriteResult::FILE_SAVED;
            }
            return WriteResult("Unable to open file for output");
        }

        virtual WriteResult writeObject(const Object& obj,std::ostream& fout, const osgDB::ReaderWriter::Options* options) const
        {
            plugin_osgfs::Output foutput;
            foutput.setOptions(options);

            std::ios &fios = foutput;
            fios.rdbuf(fout.rdbuf());

            if (fout)
            {
                setPrecision(foutput,options);

                foutput.writeObject(obj);
                return WriteResult::FILE_SAVED;
            }
            return WriteResult("Unable to write to output stream");
        }


        virtual WriteResult writeNode(const Node& node,const std::string& fileName, const osgDB::ReaderWriter::Options* options) const
        {
            std::string ext = getFileExtension(fileName);
            if (!acceptsExtension(ext)) return WriteResult::FILE_NOT_HANDLED;


            plugin_osgfs::Output fout(fileName.c_str());
            fout.setOptions(options);
            if (fout)
            {
                setPrecision(fout,options);

                fout.writeObject(node);
                fout.close();
                return WriteResult::FILE_SAVED;
            }
            return WriteResult("Unable to open file for output");
        }

        virtual WriteResult writeNode(const Node& node,std::ostream& fout, const osgDB::ReaderWriter::Options* options) const
        {
            plugin_osgfs::Output foutput;
            foutput.setOptions(options);

            std::ios &fios = foutput;
            fios.rdbuf(fout.rdbuf());

            if (fout)
            {
                setPrecision(foutput,options);

                foutput.writeObject(node);
                return WriteResult::FILE_SAVED;
            }
            return WriteResult("Unable to write to output stream");
        }

};

// now register with Registry to instantiate the above reader/writer.
RegisterReaderWriterProxy<OSGFSReaderWriter> g_OSGFSReaderWriterProxy;

// vim: set sw=4 ts=8 et ic ai:
