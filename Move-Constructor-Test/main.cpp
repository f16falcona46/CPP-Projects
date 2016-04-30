#include <iostream>
#include <TestClass.h>

TestClass dostuff() {
	TestClass t(20);
	return t;
}

int main() {
	TestClass t(10);
	TestClass u = dostuff();
	std::cout << "T: " << t << std::endl;
	std::cout << "U: " << u << std::endl;
	return 0;
}
