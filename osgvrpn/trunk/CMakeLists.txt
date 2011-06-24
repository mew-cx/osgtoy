# # # # # #  project setup  # # # # # #
CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )
PROJECT( osgVRPN )

SET( CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}" )

SET( LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single directory for all libraries." )
MAKE_DIRECTORY( ${LIBRARY_OUTPUT_PATH} )
MARK_AS_ADVANCED( LIBRARY_OUTPUT_PATH )

SET( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single directory for all executables." )
MAKE_DIRECTORY( ${EXECUTABLE_OUTPUT_PATH} )
MARK_AS_ADVANCED( EXECUTABLE_OUTPUT_PATH )

IF( WIN32 AND MSVC )
    OPTION( OSGVRPN_WIN32_USE_MP "Build with multiple processes (/MP option; VS 2005 and above)." ON )
    MARK_AS_ADVANCED( OSGVRPN_WIN32_USE_MP )
    IF( OSGVRPN_WIN32_USE_MP )
        SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP" )
    ENDIF( OSGVRPN_WIN32_USE_MP )
ENDIF( WIN32 AND MSVC )

include( CMakeMacros )
include( FindOSGHelper )
#include( FindVRPNHelper )

# Add OSG for entire project for include & lib paths
INCLUDE_DIRECTORIES( 
    ${PROJECT_SOURCE_DIR}/include
    ${OSG_INCLUDE_DIRS}
)
# HACK: Not sure why this is necessary to find debug library
LINK_DIRECTORIES(
    ${EXECUTABLE_OUTPUT_PATH}/Debug
    ${CMAKE_INSTALL_PREFIX}/lib         # hack to find libvrpn.a
)

add_subdirectory( src/osgVRPN )
add_subdirectory( src/osgPlugins )
add_subdirectory( examples )

