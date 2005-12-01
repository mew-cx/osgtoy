
TGT = osgdb_smoother.so

CXXFLAGS = -fpic -I../../../include
CXXSRC = $(wildcard *.cpp)
OBJS = $(CXXSRC:%.cpp=%.o)

LD_LIBRARY_PATH=`pwd`

all default: ${TGT}

${TGT}: ${OBJS}
	g++ -shared -fpic -o $@ $^ -losgDB -losg 

test: ${TGT}
	osgviewer ../../../data/octa.osg.smoother

clean:
	rm -f *.o

clobber: clean
	rm -f ${TGT}

