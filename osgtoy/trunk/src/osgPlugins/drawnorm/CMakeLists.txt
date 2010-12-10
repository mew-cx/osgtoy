SET( LIB_NAME osgdb_drawnorm )

ADD_OSGPLUGIN( ${LIB_NAME}
    ReaderWriterDRAWNORM.cpp
)

INCLUDE(ModuleInstall OPTIONAL)
