SET( LIB_NAME osgToy )

ADD_DEFINITIONS( -DOSGTOY_LIBRARY )
SET( HEADER_PATH ${PROJECT_SOURCE_DIR}/include/${LIB_NAME} )

if( LUA51_FOUND )
    set( _luaHeaders 
        ${HEADER_PATH}/LuaState.h
    )
    set( _luaSource
        LuaState.cpp
        lua_osg.cpp
        lua_osgToy.cpp
    )
else()
    set( _luaHeaders "" )
    set( _luaSource "" )
endif()

if( GLSL_FOUND )
    set( _glslHeaders 
        ${HEADER_PATH}/GlslLint.h
        ${HEADER_PATH}/GlslLintVisitor.h
    )
    set( _glslSource 
        GlslLint.cpp
        GlslLintVisitor.cpp
    )
else()
    set( _glslHeaders "" )
    set( _glslSource "" )
endif()

SET( LIB_PUBLIC_HEADERS
    ${HEADER_PATH}/Export.h
    ${HEADER_PATH}/FacetingVisitor.h
    ${HEADER_PATH}/Geometry.h
    ${HEADER_PATH}/LorenzAttractor.h
    ${HEADER_PATH}/MengerSponge.h
    ${HEADER_PATH}/MipmapTestImage.h
    ${HEADER_PATH}/Normals.h
    ${HEADER_PATH}/Polyhedra.h
    ${HEADER_PATH}/RungeKutta4.h
    ${HEADER_PATH}/SuperShape3D.h
    ${HEADER_PATH}/Teapot.h
    ${HEADER_PATH}/TextFileReader.h
    ${HEADER_PATH}/Version.h
    ${_luaHeaders}
    ${_glslHeaders}
)
#    ${HEADER_PATH}/SceneHelpers.h

ADD_SHARED_LIBRARY_INTERNAL( ${LIB_NAME}
    ${LIB_PUBLIC_HEADERS}
    FacetingVisitor.cpp
    Geometry.cpp
    LorenzAttractor.cpp
    MengerSponge.cpp
    MipmapTestImage.cpp
    Normals.cpp
    Polyhedra.cpp
    RungeKutta4.cpp
    SuperShape3D.cpp
    Teapot.cpp
    TextFileReader.cpp
    Version.cpp
    ${_luaSource}
    ${_glslSource}
)
#    SceneHelpers.cpp

TARGET_LINK_LIBRARIES(
    ${LIB_NAME}
    ${OSG_LIBRARIES}
    ${LUA_LIBRARIES}
)

INCLUDE(ModuleInstall OPTIONAL)

