#include <unistd.h>
#include <iostream>
#include "JoystickHandler.h"

int main()
{
	JoystickHandler handler("/dev/input/js0");
	handler.StartUpdateThread();
	while (true) {
		for (int i = 0; i < handler.GetNumButtons(); ++i) {
			std::cout << "Button " << i << ": " << handler.GetButtonState(i) << '\n';
		}
		for (int i = 0; i < handler.GetNumAxes(); ++i) {
			std::cout << "Axis " << i << ": " << handler.GetAxisState(i) << '\n';
		}
		std::cout << "\n\n";
		sleep(1);
	}
	return 0;
}
