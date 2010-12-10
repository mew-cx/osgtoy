SET( LIB_NAME osgdb_lua )

ADD_OSGPLUGIN( ${LIB_NAME}
    ReaderWriterLUA.cpp
)

INCLUDE(ModuleInstall OPTIONAL)
