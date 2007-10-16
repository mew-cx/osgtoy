
FIND_PATH( VRPN_INCLUDE_DIR "vrpn_Types.h"
    $ENV{OSG_ROOT}/include
)

FIND_LIBRARY( VRPN_LIBRARY "vrpn"
    PATHS
    $ENV{OSG_ROOT}/lib
    # 3rdParty/lib
)

FIND_LIBRARY( VRPN_LIBRARY_DEBUG "vrpn${CMAKE_DEBUG_POSTFIX}"
    PATHS
    $ENV{OSG_ROOT}/lib
    # 3rdParty/lib
)

IF( NOT VRPN_LIBRARY_DEBUG )
    IF( VRPN_LIBRARY )
        SET( VRPN_LIBRARY_DEBUG VRPN_LIBRARY )
    ENDIF( VRPN_LIBRARY )
ENDIF( NOT VRPN_LIBRARY_DEBUG)
           
SET( VRPN_FOUND "NO" )
IF( VRPN_LIBRARY AND VRPN_INCLUDE_DIR )
    SET( VRPN_FOUND "YES" )
ENDIF( VRPN_LIBRARY AND VRPN_INCLUDE_DIR )

# vim: set sw=4 ts=8 et ic ai:
