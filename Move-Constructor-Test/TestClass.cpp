#include "TestClass.h"

std::ostream& operator<<(std::ostream& os, const TestClass& c) {
	os << c.get();
	return os;
}

