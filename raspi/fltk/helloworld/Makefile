CC=gcc
CXX=g++
LINKS:=$(shell fltk-config --use-images --ldflags)
OBJS=main.o
FLAGS+=
CXXFLAGS+=-std=c++14 $(shell fltk-config --use-images --cxxflags)

#You may need -lfltk_forms -lfltk_images -lfltk_gl

.PHONY: all clean debug all_impl

all:
	$(eval CXXFLAGS+= -O2)
	$(eval export CFLAGS)
	$(eval export CXXFLAGS)
	+$(MAKE) all_impl

#before make debug, MUST make clean first!
debug:
	$(eval CXXFLAGS+= -g)
	$(eval export CFLAGS)
	$(eval export CXXFLAGS)
	+$(MAKE) all_impl

all_impl: helloworld

#WHEN ADDING A NEW CLASS
#ClassName.o: precompiled_headers.h.gch common.h ClassName.cpp ClassName.h
#	$(CXX) -c $(FLAGS) $(CXXFLAGS) $(WXCXXFLAGS) ClassName.cpp -o ClassName.o
#Note that each .o file must not only depend on its .cpp file, but also on all .h files it uses

precompiled_headers.h.gch: precompiled_headers.h
	$(CXX) $(FLAGS) $(CXXFLAGS) -c precompiled_headers.h -o precompiled_headers.h.gch

helloworld: precompiled_headers.h.gch $(OBJS)
	$(CXX) $(FLAGS) $(CXXFLAGS) -o helloworld $(OBJS) $(LINKS)
	strip helloworld

clean:
	rm -f *.o helloworld *.d *.gch

main.o: precompiled_headers.h.gch common.h main.cpp
	$(CXX) -c $(FLAGS) $(CXXFLAGS) main.cpp -o main.o
