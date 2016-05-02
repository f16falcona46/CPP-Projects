CXX=g++
CXXFLAGS=--std=c++14

DEFAULT:
	echo "Use make all to compile, or make clean to remove all compiled files."

object:
	mkdir object

bin:
	mkdir bin

object/main.o: object main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o object/main.o

object/TestClass.o: object TestClass.cpp
	$(CXX) $(CXXFLAGS) -c TestClass.cpp -o object/TestClass.o

all: object/main.o object/TestClass.o bin
	g++ object/main.o object/TestClass.o -o bin/main

clean:
	rm -rf object bin
