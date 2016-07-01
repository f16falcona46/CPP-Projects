CC=gcc

.PHONY: all clean
all: notepad_with_toolbars.exe

notepad_with_toolbars.exe: notepad_with_toolbars.o resources.o
	$(CC) -mwindows -o notepad_with_toolbars.exe notepad_with_toolbars.o resources.o -lcomctl32

notepad_with_toolbars.o: resource.h

resources.o: resources.rc Application.manifest exeicons.ico icons.ico resource.h
	windres resources.rc -O coff -o resources.o

clean:
	rm -f *.o *.exe
