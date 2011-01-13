SET( LIB_NAME osgdb_wirebox )

ADD_OSGPLUGIN( ${LIB_NAME}
	ReaderWriterWIREBOX.cpp
)

INCLUDE(ModuleInstall OPTIONAL)
