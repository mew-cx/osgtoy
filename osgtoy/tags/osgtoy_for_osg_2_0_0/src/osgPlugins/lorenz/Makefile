
TGT = osgdb_lorenz.so

CXXFLAGS = -fpic -I../../../include
CXXSRC = $(wildcard *.cpp)
OBJS = $(CXXSRC:%.cpp=%.o)

TOYPATH=../../osgToy
TOYLIB=${TOYPATH}/libosgToy.so

LD_LIBRARY_PATH=`pwd`:${TOYPATH}

all default: ${TGT}

${TGT}: ${TOYLIB} ${OBJS}
	g++ -shared -fpic -o $@ $^ -losgDB -losg -L${TOYPATH} -losgToy

${TOYLIB}:
	make -C ${TOYPATH}

test: ${TGT}
	osgviewer 20000.lorenz

clean:
	rm -f *.o

clobber: clean
	rm -f ${TGT}

