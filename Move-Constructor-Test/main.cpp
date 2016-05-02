#include <iostream>
#include "TestClass.h"
#include "Vec2.h"

TestClass dostuff(int i) {
	TestClass t(i);
	return t;
}

int main() {
	TestClass t(10);
	TestClass u = dostuff(20);
	u = dostuff(45);
	std::cout << "T: " << t << std::endl;
	std::cout << "U: " << u << std::endl;
	JLcpp::Vec2<double> v1(1,2);
	std::cout << v1 << std::endl;
	std::cout << v1.distance() << std::endl;
	std::cout << v1.angle() << std::endl;
	return 0;
}
