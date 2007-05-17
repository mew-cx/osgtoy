SET(TARGET_DEFAULT_PREFIX "example_")
SET(TARGET_DEFAULT_LABEL_PREFIX "Examples")

SET(TARGET_COMMON_LIBRARIES 
        OpenThreads
	osg 
	osgDB 
	osgUtil
	osgGA
	osgViewer 
	osgText 
)

ADD_SUBDIRECTORY(osgVRPNviewer)
