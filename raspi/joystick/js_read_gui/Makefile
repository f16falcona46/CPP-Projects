CC=gcc
CXX=g++
LINKS:=$(shell wx-config --libs) -lm
OBJS=JSReadGUI.o MainWindow.o JSPositionPanel.o ButtonStatePanel.o JoystickHandler.o
FLAGS+=
CFLAGS+=-std=c11
CXXFLAGS+=-std=c++14
WXCXXLFLAGS:=$(shell wx-config --cxxflags --cppflags --libs)
WXCXXFLAGS:= $(shell wx-config --cxxflags --cppflags)
EXENAME=js_read_gui

.PHONY: all clean debug all_impl

all:
	$(eval CFLAGS+= -O2 -DNDEBUG)
	$(eval CXXFLAGS+= -O2 -DNDEBUG)
	$(eval export CFLAGS -DNDEBUG)
	$(eval export CXXFLAGS -DNDEBUG)
	+$(MAKE) all_impl
	strip $(EXENAME)

#before make debug, MUST make clean first!
debug:
	$(eval CFLAGS+= -g)
	$(eval CXXFLAGS+= -g)
	$(eval export CFLAGS)
	$(eval export CXXFLAGS)
	+$(MAKE) all_impl

all_impl: $(EXENAME)

#WHEN ADDING A NEW CLASS
#ClassName.o: precompiled_headers.h.gch common.h ClassName.cpp ClassName.h
#	$(CXX) -c $(FLAGS) $(CXXFLAGS) $(WXCXXFLAGS) ClassName.cpp -o ClassName.o
#Note that each .o file must not only depend on its .cpp file, but also on all .h files it uses

precompiled_headers.h.gch: precompiled_headers.h
	$(CXX) $(FLAGS) $(CXXFLAGS) $(WXCXXFLAGS) -c precompiled_headers.h -o precompiled_headers.h.gch

$(EXENAME): precompiled_headers.h.gch $(OBJS)
	$(CXX) $(FLAGS) $(CXXFLAGS) $(WXCXXFLAGS) -o $(EXENAME) $(OBJS) $(LINKS)

clean:
	rm -f *.o $(EXENAME) *.d *.gch

JSReadGUI.o: precompiled_headers.h.gch common.h JSReadGUI.cpp MainWindow.h JSPositionPanel.h JSReadGUI.h JoystickHandler.h
	$(CXX) -c $(FLAGS) $(CXXFLAGS) $(WXCXXFLAGS) JSReadGUI.cpp -o JSReadGUI.o

MainWindow.o: precompiled_headers.h.gch common.h MainWindow.cpp MainWindow.h
	$(CXX) -c $(FLAGS) $(CXXFLAGS) $(WXCXXFLAGS) MainWindow.cpp -o MainWindow.o

JSPositionPanel.o: precompiled_headers.h.gch common.h JSPositionPanel.cpp JSPositionPanel.h JoystickHandler.h
	$(CXX) -c $(FLAGS) $(CXXFLAGS) $(WXCXXFLAGS) JSPositionPanel.cpp -o JSPositionPanel.o

ButtonStatePanel.o: precompiled_headers.h.gch common.h ButtonStatePanel.cpp ButtonStatePanel.h JoystickHandler.h
	$(CXX) -c $(FLAGS) $(CXXFLAGS) $(WXCXXFLAGS) ButtonStatePanel.cpp -o ButtonStatePanel.o

JoystickHandler.o: JoystickHandler.h JoystickHandler.cpp
	$(CXX) -c $(FLAGS) $(CXXFLAGS) JoystickHandler.cpp -o JoystickHandler.o
