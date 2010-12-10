SET( LIB_NAME osgdb_sponge )

ADD_OSGPLUGIN( ${LIB_NAME}
    ReaderWriterSPONGE.cpp
)

INCLUDE(ModuleInstall OPTIONAL)
