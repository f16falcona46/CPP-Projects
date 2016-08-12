#include <future>

int main() {
	std::future<void> handles[4];
	for (size_t i = 0; i < sizeof(handles); ++i) {
		handles[i] = std::async(std::launch::async, [](){while(1);});
	}
	for (size_t i = 0; i < sizeof(handles); ++i) {
		handles[i].get();
	}
	return 0;
}
