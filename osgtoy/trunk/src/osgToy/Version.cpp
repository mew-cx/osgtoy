// $Id$

#include <osgToy/Version.h>

//#include <vector>
#include <string>
#include <sstream>


#if 0
namespace {

class StringArray : public std::vector< std::string >
{
public:
    StringArray();
    ~StringArray();

    int Tokenize( const std::string& str );

private:        // disallowed
    StringArray( const StringArray& );
    StringArray& operator=( const StringArray& );
};

int StringArray::Tokenize( const std::string& str )
{
    int count(0);
    std::istringstream ss( str );
    std::string token;
    while( ss >> token )
    {
        push_back( token );
        ++count;
    }
    return count;
}

}
#endif


/////////////////////////////////////////////////////////////////////////////

const char* osgToyGetVersion()
{
    return "0.4.2";
}

const char* osgToyGetLibraryName()
{
    return "OpenSceneGraph Toy Library http://osgtoy.sourceforge.net/";
}


/////////////////////////////////////////////////////////////////////////////
// Subversion's keyword substitution must be enabled on this file using the
// following command to automatically update the string values:
//      svn propset svn:keywords "Id Date Author URL Revision" Version.cpp

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

const unsigned int osgToySvndRevision()
{
    unsigned int revision(0);
    std::string junk;
    std::istringstream ss( osgToySvnKeywordRevision() );
    ss >> junk >> revision;
    return revision;
}

// vim: set sw=4 ts=8 et ic ai:
