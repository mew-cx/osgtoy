SET( LIB_NAME osgdb_glsllint )

ADD_OSGPLUGIN( ${LIB_NAME}
    ReaderWriterGLSLLINT.cpp
)

INCLUDE(ModuleInstall OPTIONAL)
