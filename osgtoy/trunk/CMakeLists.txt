# # # # # #  project setup  # # # # # #
CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )
PROJECT( osgToy )

SET( CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}" )

SET( LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single directory for all libraries." )
MAKE_DIRECTORY( ${LIBRARY_OUTPUT_PATH} )
MARK_AS_ADVANCED( LIBRARY_OUTPUT_PATH )

SET( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single directory for all executables." )
MAKE_DIRECTORY( ${EXECUTABLE_OUTPUT_PATH} )
MARK_AS_ADVANCED( EXECUTABLE_OUTPUT_PATH )

IF( WIN32 AND MSVC )
    OPTION( OSGTOY_WIN32_USE_MP "Build with multiple processes (/MP option; VS 2005 and above)." ON )
    MARK_AS_ADVANCED( OSGTOY_WIN32_USE_MP )
    IF( OSGTOY_WIN32_USE_MP )
        SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP" )
    ENDIF( OSGTOY_WIN32_USE_MP )
ENDIF( WIN32 AND MSVC )


include( CMakeMacros )


include( FindOSGHelper )
find_package( Lua51 )

#DEPRECATED	find_package( GLSL )
#DEPRECATED	find_package( PRODUCER )



# Add OSG for entire project for include & lib paths
INCLUDE_DIRECTORIES( 
    ${PROJECT_SOURCE_DIR}/include
    ${OSG_INCLUDE_DIRS}
    ${LUA_INCLUDE_DIR}
)
# HACK: Not sure why this is necessary, but linker can't find osgToyd.lib without it.
LINK_DIRECTORIES(
    ${EXECUTABLE_OUTPUT_PATH}/Debug
)


option( OSGTOY_BUILD_APPLICATIONS "Enable to build applications." ON )
if( OSGTOY_BUILD_APPLICATIONS )
    add_subdirectory( applications )
endif()

add_subdirectory( src/osgToy )
add_subdirectory( src/osgPlugins )

