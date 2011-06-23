SET( LIB_NAME osgdb_tracker )

ADD_OSGPLUGIN( ${LIB_NAME}
    ReaderWriterTRACKER.cpp
)

INCLUDE(ModuleInstall OPTIONAL)
