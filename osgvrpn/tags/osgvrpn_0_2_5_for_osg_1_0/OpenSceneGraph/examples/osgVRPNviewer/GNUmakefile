TOPDIR = ../..
include $(TOPDIR)/Make/makedefs

CXXFILES =\
	osgVRPNviewer.cpp\

LIBS     += -losgVRPN -losgProducer $(OSG_LIBS) $(GL_LIBS) $(X_LIBS) $(OTHER_LIBS) 

INSTFILES = \
	$(CXXFILES)\
	GNUmakefile.inst=GNUmakefile

EXEC = osgVRPNviewer

include $(TOPDIR)/Make/makerules
