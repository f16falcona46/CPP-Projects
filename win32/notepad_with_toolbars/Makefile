CC=gcc
CXX=g++
LINKS=-lcomctl32
OBJS=resources.o notepad_with_toolbars.o
FLAGS+=-g
CFLAGS+=-std=c11
CXXFLAGS+=-std=c++14

.PHONY: all clean
all: notepad_with_toolbars.exe

notepad_with_toolbars.exe: $(OBJS)
	$(CXX) $(FLAGS) $(CXXFLAGS) -mwindows -o notepad_with_toolbars.exe $(OBJS) $(LINKS)

resources.o: resources.rc Application.manifest exeicons.ico icons.ico resource.h
	windres resources.rc -O coff -o resources.o

clean:
	rm -f *.o *.exe *.d

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
