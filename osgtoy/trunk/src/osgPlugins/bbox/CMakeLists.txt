SET( LIB_NAME osgdb_bbox )

ADD_OSGPLUGIN( ${LIB_NAME}
    ReaderWriterBBOX.cpp
)

INCLUDE(ModuleInstall OPTIONAL)
