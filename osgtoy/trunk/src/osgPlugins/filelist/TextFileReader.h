// textfilereader.h
// mew 2011-01-09

#ifndef TEXTFILEREADER_H
#define TEXTFILEREADER_H

#include <string>

class TextFileReader
{
public:
    TextFileReader();
    virtual ~TextFileReader() {}

    bool readFile( const std::string& fileName );
    bool readFile( const char* fileName );

protected:        // methods
    virtual void stripComment( std::string& line );
    virtual void processLine( const std::string& line );

private:        // disallowed
    TextFileReader( const TextFileReader& );
    TextFileReader& operator=( const TextFileReader& );
};

#endif


// vim: set sw=4 ts=8 et ic ai:
