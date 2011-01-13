SET( LIB_NAME osgdb_filelist )

ADD_OSGPLUGIN( ${LIB_NAME}
    TextFileReader.h
    TextFileReader.cpp
    ReaderWriterFILELIST.cpp
)

INCLUDE(ModuleInstall OPTIONAL)
