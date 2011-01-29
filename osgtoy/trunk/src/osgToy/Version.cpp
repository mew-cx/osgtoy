// $Id$
// Subversion's keyword substitution must be enabled to automatically update these values:
//      svn propset svn:keywords "Id Date Author URL Revision" Version.cpp

#include <osgToy/Version.h>


const char* osgToyGetVersion()
{
    return "0.4.0";
}

const char* osgToyGetLibraryName()
{
    return "Open Scene Graph Toy Library from http://mew.cx/";
}



const char* osgToySvnKeywordId()
{
    return "$Id$";
}

const char* osgToySvnKeywordDate()
{
    return "$Date$";
}

const char* osgToySvnKeywordAuthor()
{
    return "$Author$";
}

const char* osgToySvnKeywordUrl()
{
    return "$URL$";
}

const char* osgToySvnKeywordRevision()
{
    return "$Revision$";
}


// vim: set sw=4 ts=8 et ic ai:
