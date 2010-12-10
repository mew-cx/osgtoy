SET( LIB_NAME osgdb_smoother )

ADD_OSGPLUGIN( ${LIB_NAME}
    ReaderWriterSMOOTHER.cpp
)

INCLUDE(ModuleInstall OPTIONAL)
