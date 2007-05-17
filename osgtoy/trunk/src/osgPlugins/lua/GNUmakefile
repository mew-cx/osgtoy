TOPDIR = ../../..
include $(TOPDIR)/Make/makedefs 

CXXFILES =\
	ReaderWriterLUA.cpp\

LIBS   += $(OSG_LIBS) $(OTHER_LIBS)

TARGET_BASENAME = lua
include $(TOPDIR)/Make/cygwin_plugin_def
PLUGIN = $(PLUGIN_PREFIX)$(TARGET_BASENAME).$(PLUGIN_EXT)

include $(TOPDIR)/Make/makerules 
