CC=gcc

.PHONY: all clean
all: window_test.exe

window_test.exe: window_test.o resources.o
	$(CC) -mwindows -o window_test.exe window_test.o resources.o

resources.o: resources.rc Application.manifest exeicons.ico icons.ico
	windres resources.rc -O coff -o resources.o

clean:
	rm -f *.o *.exe
