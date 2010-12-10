SET( LIB_NAME osgdb_image )

ADD_OSGPLUGIN( ${LIB_NAME}
    ReaderWriterIMAGE.cpp
)

INCLUDE(ModuleInstall OPTIONAL)
