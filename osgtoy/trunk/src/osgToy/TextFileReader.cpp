// TextFileReader.cpp
// mew 2011-01-30
// A super-simple 1-line-at-a-time textfile reader.
// Trims out "#"-prefixed comments and prefix/suffix whitespace cruft
// from each line of text before calling your evaluate() method.


#include <osgToy/TextFileReader.h>      // our interface definition

#include <fstream>
#include <sstream>
#include <iostream>
#include <cassert>


// public ///////////////////////////////////////////////////////////////////

osgToy::TextFileReader::TextFileReader()
{
}

bool osgToy::TextFileReader::readFile( const std::string& fileName )
{
    return readFile( fileName.c_str() );
}

bool osgToy::TextFileReader::readFile( const char* fileName )
{
    std::ifstream configFile;
    configFile.open( fileName );

    if( ! configFile )
        return false;

    while( configFile.good() )
    {
        std::string text;
        std::getline( configFile, text );
        trim( text );
        evaluate( text );
    }

    configFile.close();
    return true;
}


// protected ////////////////////////////////////////////////////////////////

// Trim comments, whitespace and other inert matter from the text string.
void osgToy::TextFileReader::trim( std::string& text )
{
    // Truncate text at first comment-prefix character occurance.
    // IMPORTANT: This implementation completely ignores any quote or escape
    // protection of the comment-prefix character!!
    size_t commentPos( text.find( '#', 0 ) );
    if ( commentPos != std::string::npos )
    {
        text.erase( commentPos );
    }

    // trim leading/trailing whitespace from text.
    std::stringstream trimmer;
    trimmer << text;
    text.clear();
    trimmer >> text;
}


#if 1
// do something with the line of text.
void osgToy::TextFileReader::evaluate( std::string& text )
{
    if( text.empty() )
        return;

    // tokenization example:
    std::istringstream textStream( text );
    std::string token0, token1;
    textStream >> token0 >> token1;
    std::cout << "token0 = \"" << token0 << "\"" << std::endl;
    std::cout << "token1 = \"" << token1 << "\"" << std::endl;
#endif

}


// vim: set sw=4 ts=8 et ic ai:
