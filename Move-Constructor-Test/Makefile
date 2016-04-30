DEFAULT:
	echo "Use make all to compile, or make clean to remove all compiled files."

object:
	mkdir object

bin:
	mkdir bin

object/main.o: object
	g++ -c --std=c++14 main.cpp -o object/main.o

object/TestClass.o: object
	g++ -c --std=c++14 TestClass.cpp -o object/TestClass.o

all: object/main.o object/TestClass.o bin
	g++ object/main.o object/TestClass.o -o bin/main.exe

clean:
	rm -rf object bin
