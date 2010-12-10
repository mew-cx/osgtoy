SET( LIB_NAME osgdb_lorenz )

ADD_OSGPLUGIN( ${LIB_NAME}
    ReaderWriterLORENZ.cpp
)

INCLUDE(ModuleInstall OPTIONAL)
