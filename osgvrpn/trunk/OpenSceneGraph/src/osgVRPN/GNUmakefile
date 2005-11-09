TOPDIR = ../..
include $(TOPDIR)/Make/makedefs 

CXXFILES =\
	Analog.cpp \
	AnalogManipulator.cpp \
	AnalogTransform.cpp \
	Tracker.cpp \
	TrackerManipulator.cpp \
	TrackerTransform.cpp \
	Version.cpp \

LIBS += /usr/local/lib/libvrpn.a -losgGA -losg $(OTHER_LIBS)
DEF  += -DOSGVRPN_LIBRARY
TARGET_BASENAME = osgVRPN
LIB = $(LIB_PREFIX)$(TARGET_BASENAME).$(LIB_EXT)

include $(TOPDIR)/Make/makerules 
