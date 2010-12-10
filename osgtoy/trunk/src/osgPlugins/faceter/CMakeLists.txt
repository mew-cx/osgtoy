SET( LIB_NAME osgdb_faceter )

ADD_OSGPLUGIN( ${LIB_NAME}
    ReaderWriterFACETER.cpp
)

INCLUDE(ModuleInstall OPTIONAL)
