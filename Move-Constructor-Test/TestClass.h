#ifndef JLCPP_TESTCLASS_H
#define JLCPP_TESTCLASS_H

#include <iostream>

class TestClass {
	private:
	int x;
	public:
	int get() const { return x; }
	void set(int new_x) { this->x = new_x; }
	TestClass(TestClass& other) {
		this->x = other.get();
		std::cout << "Copy constructor" << std::endl;
	}
	TestClass& operator=(TestClass& other) {
		this->x = other.get();
		std::cout << "Copy assignment" << std::endl;
	}
	TestClass(TestClass&& other) {
		this->x = other.get();
		std::cout << "Move constructor" << std::endl;
	}
	TestClass& operator=(TestClass&& other) {
		this->x = other.get();
		std::cout << "Move assignment" << std::endl;
	}
	TestClass(int newx) {
		this->x = newx;
	}
};

std::ostream& operator<<(std::ostream& os, const TestClass& c);
#endif
