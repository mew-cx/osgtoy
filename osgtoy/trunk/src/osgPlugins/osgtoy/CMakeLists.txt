SET( LIB_NAME osgdb_osgtoy )

ADD_OSGPLUGIN( ${LIB_NAME}
    ReaderWriterOSGTOY.cpp
)

INCLUDE(ModuleInstall OPTIONAL)
