#include <iostream>
#include "TestClass.h"

TestClass dostuff(int i) {
	TestClass t(i);
	return t;
}

int main() {
	TestClass t(10);
	TestClass u = dostuff(20);
	u = dostuff(40);
	std::cout << "T: " << t << std::endl;
	std::cout << "U: " << u << std::endl;
	return 0;
}
