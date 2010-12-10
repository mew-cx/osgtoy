SET( LIB_NAME osgdb_simplifier )

ADD_OSGPLUGIN( ${LIB_NAME}
    ReaderWriterSIMPLIFIER.cpp
)

INCLUDE(ModuleInstall OPTIONAL)
