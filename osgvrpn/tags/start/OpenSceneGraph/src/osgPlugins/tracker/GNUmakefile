TOPDIR = ../../..
include $(TOPDIR)/Make/makedefs 

CXXFILES =\
	ReaderWriterTRACKER.cpp\

LIBS   += -losgVRPN $(OSG_LIBS) $(OTHER_LIBS)

TARGET_BASENAME = tracker
include $(TOPDIR)/Make/cygwin_plugin_def
PLUGIN = $(PLUGIN_PREFIX)$(TARGET_BASENAME).$(PLUGIN_EXT)

include $(TOPDIR)/Make/makerules 
