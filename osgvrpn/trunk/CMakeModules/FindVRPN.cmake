# This module defines
# VRPN_LIBRARY
# VRPN_FOUND, if false, do not try to link
# VRPN_INCLUDE_DIR, where to find the headers
#
# $VRPN_DIR is an environment variable that would
# correspond to the ./configure --prefix=$DELTA3D
#
# derived from dtGauge


FIND_PATH( VRPN_INCLUDE_DIR vrpn_something.h
    ${VRPN_DIR}/include
    $ENV{DELTA_ROOT}/inc
    ~/Library/Frameworks
    /Library/Frameworks
    /usr/local/include
    /usr/include
    /sw/include # Fink
    /opt/local/include # DarwinPorts
    /opt/csw/include # Blastwave
    /opt/include
    [HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session\ Manager\\Environment;DELTA_ROOT]/inc
    /usr/freeware/include
)

MACRO( FIND_VRPN_LIBRARY MYLIBRARY MYLIBRARYNAME )

    FIND_LIBRARY("${MYLIBRARY}_DEBUG"
        NAMES "${MYLIBRARYNAME}${CMAKE_DEBUG_POSTFIX}"
        PATHS
        ${VRPN_DIR}/lib/Debug
        ${VRPN_DIR}/lib
        $ENV{VRPN_DIR}/lib/debug
        $ENV{VRPN_DIR}/lib
        $ENV{VRPN_DIR}
        $ENV{DELTA_ROOT}/lib
        ~/Library/Frameworks
        /Library/Frameworks
        /usr/local/lib
        /usr/lib
        /sw/lib
        /opt/local/lib
        /opt/csw/lib
        /opt/lib
        [HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session\ Manager\\Environment;DELTA_ROOT]/lib
        /usr/freeware/lib64
    )

    FIND_LIBRARY(${MYLIBRARY}
        NAMES ${MYLIBRARYNAME}
        PATHS
        ${VRPN_DIR}/lib/Release
        ${VRPN_DIR}/lib
        $ENV{VRPN_DIR}/lib/Release
        $ENV{VRPN_DIR}/lib
        $ENV{VRPN_DIR}
        $ENV{DELTA_ROOT}/lib
        ~/Library/Frameworks
        /Library/Frameworks
        /usr/local/lib
        /usr/lib
        /sw/lib
        /opt/local/lib
        /opt/csw/lib
        /opt/lib
        [HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session\ Manager\\Environment;DELTA_ROOT]/lib
        /usr/freeware/lib64
    )
    IF( NOT ${MYLIBRARY}_DEBUG)
        IF(MYLIBRARY)
            SET(${MYLIBRARY}_DEBUG ${MYLIBRARY})
         ENDIF(MYLIBRARY)
    ENDIF( NOT ${MYLIBRARY}_DEBUG)
           
ENDMACRO( FIND_VRPN_LIBRARY LIBRARY LIBRARYNAME )

FIND_VRPN_LIBRARY( VRPN_LIBRARY vrpn )

SET( VRPN_FOUND "NO" )
IF( VRPN_LIBRARY AND VRPN_INCLUDE_DIR )
    SET( VRPN_FOUND "YES" )
ENDIF( VRPN_LIBRARY AND VRPN_INCLUDE_DIR )

