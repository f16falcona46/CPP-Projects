CC=gcc

.PHONY: all clean
all: window_test.exe

window_test.exe: window_test.o resources.o
	$(CC) -o window_test.exe window_test.o resources.o

resources.o: resources.rc
	windres resources.rc resources.o

clean:
	rm -f *.o *.exe
