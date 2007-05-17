SET(TARGET_DEFAULT_PREFIX "example_")
SET(TARGET_DEFAULT_LABEL_PREFIX "Examples")

SET(TARGET_COMMON_LIBRARIES 
    #    osgDB 
    #    osgText 
    osg 
    OpenThreads
)

ADD_SUBDIRECTORY(osgVRPNviewer)

# vim: set sw=4 ts=8 et ic ai:
