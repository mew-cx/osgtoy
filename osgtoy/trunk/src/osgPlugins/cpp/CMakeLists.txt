SET( LIB_NAME osgdb_cpp )

ADD_OSGPLUGIN( ${LIB_NAME}
    ReaderWriterCPP.cpp
    Image.cpp
    Image.h
)

INCLUDE(ModuleInstall OPTIONAL)
