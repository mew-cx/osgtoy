// TextFileReader.cpp
// mew 2011-01-09

#include "TextFileReader.h"      // our interface definition

#include <fstream>
#include <sstream>
#include <iostream>
#include <cassert>


// public ///////////////////////////////////////////////////////////////////

TextFileReader::TextFileReader()
{
}

bool TextFileReader::readFile( const std::string& fileName )
{
    readFile( fileName.c_str() );
}

bool TextFileReader::readFile( const char* fileName )
{
    std::ifstream configFile;
    configFile.open( fileName );

    if( ! configFile )
        return false;

    while( configFile.good() )
    {
        std::string line;
        std::getline( configFile, line );
        stripComment( line );
        processLine( line );
    }

    configFile.close();
    return true;
}


// protected ////////////////////////////////////////////////////////////////

// Remove comments or other inert matter from the text string.
void TextFileReader::stripComment( std::string& line )
{
    // Simply truncate line at first comment-prefix character occurance.
    // IMPORTANT: This implementation completely ignores any quote or escape
    // protection of the comment-prefix character!!

    size_t commentPos( line.find( '#', 0 ) );
    if ( commentPos != std::string::npos )
    {
        line.erase( commentPos );
    }
}


void TextFileReader::processLine( const std::string& line )
{
    if( ! line.empty() )
    {
        std::cout << "read line \"" << line << "\"" << std::endl;
    }

#if 0
    // possible tokenization approach:
    std::istringstream lineStream( line );
    std::string token0;
    lineStream >> token0;
#endif

}


// vim: set sw=4 ts=8 et ic ai:
