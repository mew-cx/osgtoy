SET( LIB_NAME osgdb_globe )

ADD_OSGPLUGIN( ${LIB_NAME}
    ReaderWriterGLOBE.cpp
)

INCLUDE(ModuleInstall OPTIONAL)
