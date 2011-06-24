SET( LIB_NAME osgVRPN )

ADD_DEFINITIONS( -DOSGVRPN_LIBRARY )
SET( HEADER_PATH ${PROJECT_SOURCE_DIR}/include/${LIB_NAME} )

SET( LIB_PUBLIC_HEADERS
    ${HEADER_PATH}/Analog.h
    ${HEADER_PATH}/AnalogTracker.h
    ${HEADER_PATH}/Button.h
    ${HEADER_PATH}/Export.h
    ${HEADER_PATH}/TrackerBase.h
    ${HEADER_PATH}/Tracker.h
    ${HEADER_PATH}/TrackerManipulator.h
    ${HEADER_PATH}/TrackerTransform.h
    ${HEADER_PATH}/Version.h
)

ADD_SHARED_LIBRARY_INTERNAL( ${LIB_NAME}
    ${LIB_PUBLIC_HEADERS}
    Analog.cpp
    AnalogTracker.cpp
    Button.cpp
    CMakeLists.txt
    Tracker.cpp
    TrackerManipulator.cpp
    TrackerTransform.cpp
    Version.cpp
    Version.cpp
)

TARGET_LINK_LIBRARIES( ${LIB_NAME}
    ${OSG_LIBRARIES}
    libvrpn.a
)

INCLUDE(ModuleInstall OPTIONAL)

