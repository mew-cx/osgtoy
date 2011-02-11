// $Id$
// A super-simple 1-line-at-a-time textfile reader.
// By default, trims out "#"-prefixed comments and prefix/suffix whitespace
// cruft from each line of text before calling your evaluate() method.

#ifndef OSGTOY_TEXTFILEREADER_H
#define OSGTOY_TEXTFILEREADER_H

#include <string>
#include <osgToy/Export.h>

namespace osgToy {

class OSGTOY_EXPORT TextFileReader
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

}

#endif


// vim: set sw=4 ts=8 et ic ai:
