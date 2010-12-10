SET( LIB_NAME osgdb_spin )

ADD_OSGPLUGIN( ${LIB_NAME}
    ReaderWriterSPIN.cpp
)

INCLUDE(ModuleInstall OPTIONAL)
