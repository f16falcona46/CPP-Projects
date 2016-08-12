CC=gcc
CXX=g++
LINKS:=$(shell wx-config --libs)
OBJS=main.o
FLAGS+=
CFLAGS+=-std=c11
CXXFLAGS+=-std=c++14
WXCXXFLAGS:=$(shell wx-config --cxxflags --cppflags --libs)
CXXFLAGS+= $(shell wx-config --cxxflags --cppflags)

.PHONY: all clean
all: button_test

precompiled_headers.h.gch: precompiled_headers.h
	$(CXX) $(FLAGS) $(CXXFLAGS) $(WXCXXFLAGS) -c precompiled_headers.h -o precompiled_headers.h.gch

button_test: precompiled_headers.h.gch $(OBJS)
	$(CXX) $(FLAGS) $(CXXFLAGS) $(WXCXXFLAGS) -o button_test $(OBJS) $(LINKS)

clean:
	rm -f *.o button_test *.d *.gch

#Scott McPeak's code
%.o: %.c
	$(CC) -c $(FLAGS) $(CFLAGS) $*.c -o $*.o
	$(CC) -MM $(FLAGS) $(CFLAGS) $*.c > $*.d
	@mv -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp

%.o: %.cpp
	$(CXX) -c $(FLAGS) $(CXXFLAGS) $*.cpp -o $*.o
	$(CXX) -MM $(FLAGS) $(CXXFLAGS) $*.cpp > $*.d
	@mv -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp
