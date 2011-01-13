SET( LIB_NAME osgdb_csv )

ADD_OSGPLUGIN( ${LIB_NAME}
    ReaderWriterCSV.cpp
)

INCLUDE(ModuleInstall OPTIONAL)
