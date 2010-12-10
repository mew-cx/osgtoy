SET( LIB_NAME osgdb_osgviewer )

ADD_OSGPLUGIN( ${LIB_NAME}
    ReaderWriterOSGVIEWER.cpp
)

INCLUDE(ModuleInstall OPTIONAL)
