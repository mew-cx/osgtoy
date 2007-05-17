
TGT = osgdb_spin.so

CXXFLAGS = -fpic -I../../../include
CXXSRC = $(wildcard *.cpp)
OBJS = $(CXXSRC:%.cpp=%.o)

LD_LIBRARY_PATH=`pwd`

all default: ${TGT}

${TGT}: ${OBJS}
	g++ -shared -fpic -o $@ $^ -losgDB -losg 

test: ${TGT}
	osgviewer cow.osg.spin cessna.osg

clean:
	rm -f *.o

clobber: clean
	rm -f ${TGT}

