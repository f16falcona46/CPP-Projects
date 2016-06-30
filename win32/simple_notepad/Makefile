CC=gcc

.PHONY: all clean
all: simple_notepad.exe

simple_notepad.exe: simple_notepad.o resources.o
	$(CC) -mwindows -o simple_notepad.exe simple_notepad.o resources.o

resources.o: resources.rc Application.manifest exeicons.ico icons.ico
	windres resources.rc -O coff -o resources.o

clean:
	rm -f *.o *.exe
