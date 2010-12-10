SET( LIB_NAME osgdb_movie )

ADD_OSGPLUGIN( ${LIB_NAME}
    ReaderWriterMOVIE.cpp
)

INCLUDE(ModuleInstall OPTIONAL)
