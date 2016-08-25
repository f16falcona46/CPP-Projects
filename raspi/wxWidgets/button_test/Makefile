CC=gcc
CXX=g++
LINKS:=$(shell wx-config --libs) -lm
OBJS=main.o MainWindow.o DrawingPanel.o
FLAGS+=
CFLAGS+=-std=c11
CXXFLAGS+=-std=c++14
WXCXXLFLAGS:=$(shell wx-config --cxxflags --cppflags --libs)
WXCXXFLAGS:= $(shell wx-config --cxxflags --cppflags)

.PHONY: all clean debug
all: button_test

#before make debug, MUST make clean first!
debug:
	$(eval CFLAGS+= -g)
	$(eval CXXFLAGS+= -g)
	$(eval export CFLAGS)
	$(eval export CXXFLAGS)
	make all

#WHEN ADDING A NEW CLASS
#ClassName.o: precompiled_headers.h.gch common.h ClassName.cpp ClassName.h
#	$(CXX) -c $(FLAGS) $(CXXFLAGS) $(WXCXXFLAGS) ClassName.cpp -o ClassName.o
#Note that each .o file must not only depend on its .cpp file, but also on all .h files it uses

precompiled_headers.h.gch: precompiled_headers.h
	$(CXX) $(FLAGS) $(CXXFLAGS) $(WXCXXFLAGS) -c precompiled_headers.h -o precompiled_headers.h.gch

button_test: precompiled_headers.h.gch $(OBJS)
	$(CXX) $(FLAGS) $(CXXFLAGS) $(WXCXXFLAGS) -o button_test $(OBJS) $(LINKS)

clean:
	rm -f *.o button_test *.d *.gch

main.o: precompiled_headers.h.gch common.h main.cpp MainWindow.h DrawingPanel.h
	$(CXX) -c $(FLAGS) $(CXXFLAGS) $(WXCXXFLAGS) main.cpp -o main.o

MainWindow.o: precompiled_headers.h.gch common.h MainWindow.cpp MainWindow.h
	$(CXX) -c $(FLAGS) $(CXXFLAGS) $(WXCXXFLAGS) MainWindow.cpp -o MainWindow.o

DrawingPanel.o: precompiled_headers.h.gch common.h DrawingPanel.cpp DrawingPanel.h
	$(CXX) -c $(FLAGS) $(CXXFLAGS) $(WXCXXFLAGS) DrawingPanel.cpp -o DrawingPanel.o