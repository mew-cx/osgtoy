SET( LIB_NAME osgdb_osgfs )

ADD_OSGPLUGIN( ${LIB_NAME}
    ReaderWriterOSGFS.cpp
    Input.cpp
    Input.h
    Output.cpp
    Output.h
)

INCLUDE(ModuleInstall OPTIONAL)
