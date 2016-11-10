CC=gcc
CXX=g++
LINKS:=$(shell fltk-config --use-images --ldflags)
OBJS=main.o ClockPanel.o
EXENAME=clock
FLAGS+=
CXXFLAGS+=-std=c++14 $(shell fltk-config --use-images --cxxflags)

#You may need -lfltk_forms -lfltk_images -lfltk_gl

.PHONY: all clean debug all_impl

all:
	$(eval CXXFLAGS+= -O2 -DNDEBUG)
	$(eval export CFLAGS)
	$(eval export CXXFLAGS)
	+$(MAKE) all_impl
	strip $(EXENAME)

#before make debug, MUST make clean first!
debug:
	$(eval CXXFLAGS+= -g -DDEBUG)
	$(eval export CFLAGS)
	$(eval export CXXFLAGS)
	+$(MAKE) all_impl

all_impl: $(EXENAME)

#WHEN ADDING A NEW CLASS
#ClassName.o: precompiled_headers.h.gch common.h ClassName.cpp ClassName.h
#	$(CXX) -c $(FLAGS) $(CXXFLAGS) $(WXCXXFLAGS) ClassName.cpp -o ClassName.o
#Note that each .o file must not only depend on its .cpp file, but also on all .h files it uses

$(EXENAME): $(OBJS)
	$(CXX) $(FLAGS) $(CXXFLAGS) -o $(EXENAME) $(OBJS) $(LINKS)

clean:
	rm -f ./*.o ./$(EXENAME) ./$(EXENAME).exe ./*.d ./*.gch

main.o: main.cpp
	$(CXX) -c $(FLAGS) $(CXXFLAGS) main.cpp -o main.o

ClockPanel.o: ClockPanel.cpp ClockPanel.h
	$(CXX) -c $(FLAGS) $(CXXFLAGS) ClockPanel.cpp -o ClockPanel.o
