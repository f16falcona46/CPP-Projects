CC=gcc

.PHONY: all clean
all: simple_notepad.exe

simple_notepad.exe: simple_notepad.o resources.o
	$(CC) -mwindows -o simple_notepad.exe simple_notepad.o resources.o

simple_notepad.o: resource.h

resources.o: resources.rc Application.manifest exeicons.ico icons.ico resource.h
	windres resources.rc -O coff -o resources.o

clean:
	rm -f *.o *.exe
