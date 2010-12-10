SET( LIB_NAME osgToy )

ADD_DEFINITIONS( -DOSGTOY_LIBRARY )
SET( HEADER_PATH ${PROJECT_SOURCE_DIR}/include/${LIB_NAME} )

if( LUA51_FOUND )
    set( _luaHeaders 
        ${HEADER_PATH}/LuaState
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
        ${HEADER_PATH}/GlslLint
        ${HEADER_PATH}/GlslLintVisitor
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
    ${HEADER_PATH}/Export
    ${HEADER_PATH}/FacetingVisitor
    ${HEADER_PATH}/LorenzAttractor
    ${HEADER_PATH}/MengerSponge
    ${HEADER_PATH}/MipmapTestImage
    ${HEADER_PATH}/Normals
    ${HEADER_PATH}/Polyhedra
    ${HEADER_PATH}/RungeKutta4
    ${HEADER_PATH}/SuperShape3D
    ${HEADER_PATH}/Version
    ${_luaHeaders}
    ${_glslHeaders}
)

ADD_SHARED_LIBRARY_INTERNAL( ${LIB_NAME}
    ${LIB_PUBLIC_HEADERS}
    FacetingVisitor.cpp
    LorenzAttractor.cpp
    MengerSponge.cpp
    MipmapTestImage.cpp
    Normals.cpp
    Polyhedra.cpp
    RungeKutta4.cpp
    SuperShape3D.cpp
    Version.cpp
    ${_luaSource}
    ${_glslSource}
)

TARGET_LINK_LIBRARIES(
    ${LIB_NAME}
    ${OSG_LIBRARIES}
    ${LUA_LIBRARIES}
)

INCLUDE(ModuleInstall OPTIONAL)
