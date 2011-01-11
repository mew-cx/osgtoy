SET( LIB_NAME osgdb_filelist )

ADD_OSGPLUGIN( ${LIB_NAME}
    ReaderWriterFILELIST.cpp
)

INCLUDE(ModuleInstall OPTIONAL)
