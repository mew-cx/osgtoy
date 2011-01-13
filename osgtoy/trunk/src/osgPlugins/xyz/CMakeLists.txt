SET( LIB_NAME osgdb_xyz )

ADD_OSGPLUGIN( ${LIB_NAME}
    ReaderWriterXYZ.cpp
)

INCLUDE(ModuleInstall OPTIONAL)
