
IF   (DYNAMIC_OPENSCENEGRAPH)
    ADD_DEFINITIONS(-DOSGVRPN_LIBRARY)
ELSE (DYNAMIC_OPENSCENEGRAPH)
    ADD_DEFINITIONS(-DOSG_LIBRARY_STATIC)
ENDIF(DYNAMIC_OPENSCENEGRAPH)

SET(LIB_NAME osgVRPN)
SET(HEADER_PATH ${osgVRPN_SOURCE_DIR}/include/${LIB_NAME})
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
    Analog.cpp
    AnalogTracker.cpp
    Button.cpp
    Tracker.cpp
    TrackerManipulator.cpp
    TrackerTransform.cpp
    Version.cpp
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
