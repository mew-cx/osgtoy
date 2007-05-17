
IF   (DYNAMIC_OPENSCENEGRAPH)
    ADD_DEFINITIONS(-DOSGTOY_LIBRARY)
ELSE (DYNAMIC_OPENSCENEGRAPH)
    ADD_DEFINITIONS(-DOSG_LIBRARY_STATIC)
ENDIF(DYNAMIC_OPENSCENEGRAPH)

SET(LIB_NAME osgToy)
SET(HEADER_PATH ${osgToy_SOURCE_DIR}/include/${LIB_NAME})
SET(LIB_PUBLIC_HEADERS
    ${HEADER_PATH}/Analog.h
    ${HEADER_PATH}/AnalogTracker.h
    ${HEADER_PATH}/Button.h
    ${HEADER_PATH}/Tracker.h
    ${HEADER_PATH}/TrackerBase.h
    ${HEADER_PATH}/TrackerManipulator.h
    ${HEADER_PATH}/TrackerTransform.h
)


# FIXME: For OS X, need flag for Framework or dylib
ADD_LIBRARY(${LIB_NAME}
    ${OPENSCENEGRAPH_USER_DEFINED_DYNAMIC_OR_STATIC}
    ${LIB_PUBLIC_HEADERS}
    FacetingVisitor.cpp
    LuaState.cpp
    Version.cpp
    lua_osgToy.cpp
    MipmapTestImage.cpp
    SuperShape3D.cpp
    Polyhedra.cpp
    GlslLint.cpp
    GlslLintVisitor.cpp
    MengerSponge.cpp
    lua_osg.cpp
    Normals.cpp
    Makefile
    LorenzAttractor.cpp
    RungeKutta4.cpp
)


LINK_INTERNAL(${LIB_NAME}
    #    osgDB
    #    osgText
    osg
    OpenThreads
)
LINK_CORELIB_DEFAULT(${LIB_NAME})

INCLUDE(ModuleInstall OPTIONAL)

# vim: set sw=4 ts=8 et ic ai:
