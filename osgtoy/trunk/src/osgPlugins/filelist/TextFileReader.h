// textfilereader.h
// mew 2011-01-28
// A super-simple 1-line-at-a-time textfile reader.
// Trims out "#"-prefixed comments and prefix/suffix whitespace cruft
// from each line of text before calling your evaluate() method.

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
    virtual void trim( std::string& text );
    virtual void evaluate( std::string& text ) = 0;

private:        // disallowed
    TextFileReader( const TextFileReader& );
    TextFileReader& operator=( const TextFileReader& );
};

#endif


// vim: set sw=4 ts=8 et ic ai:
