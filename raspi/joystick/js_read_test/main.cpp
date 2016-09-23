#include <time.h>
#include <iostream>
#include "JoystickHandler.h"

int main()
{
	JoystickHandler handler("/dev/input/js0");
	handler.StartUpdateThread();
	timespec sleeptime;
	sleeptime.tv_sec = 0;
	sleeptime.tv_nsec = 500000000;
	while (true) {
		for (int i = 0; i < handler.GetNumButtons(); ++i) {
			std::cout << "Button " << i << ": " << handler.GetButtonState(i) << '\n';
		}
		for (int i = 0; i < handler.GetNumAxes(); ++i) {
			std::cout << "Axis " << i << ": " << handler.GetAxisState(i) << '\n';
		}
		std::cout << "\n\n";
		nanosleep(&sleeptime, NULL);
	}
	return 0;
}
