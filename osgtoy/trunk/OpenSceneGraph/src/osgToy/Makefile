
TGT = libosgToy.so

CXXFLAGS = -fpic -I../../include
CXXSRC = $(wildcard *.cpp)
OBJS = $(CXXSRC:%.cpp=%.o)


all default: ${TGT}

${TGT}: ${OBJS}
	g++ -shared -fpic -o $@ $^ -losgDB -losg

clean:
	rm -f *.o

clobber: clean
	rm -f ${TGT}

