DLLNAME=serialization

CXX=clang++ -m32
CC=clang -m32

DLL_SRCDIR=.
DEFINES =

SRCS = pugixml.cpp main.cpp
OBJS = $(patsubst %.cpp,%.o,$(SRCS))

OPTFLAGS=-march=native -O3
CXXFLAGS=-std=gnu++11
CFLAGS=

INCLUDES=-I.
LDFLAGS = -Wl,--no-warn-mismatch -Wl,--no-undefined

%.o : %.cpp
	$(CXX) $(OPTFLAGS) $(CXXFLAGS) $(INCLUDES) $(DEFINES) -c "$<" -o $(PWD)/"$@"

$(DLLNAME) : $(OBJS)
	@mkdir -p bin/
	$(CXX) -o bin/$(DLLNAME) $(OPTFLAGS) $(LDFLAGS) $(OBJS) $(LIBS)
clean:
	-rm -f $(OBJS)
	-rm -f $(DLLNAME)
