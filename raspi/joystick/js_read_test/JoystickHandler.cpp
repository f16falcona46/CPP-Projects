#include "JoystickHandler.h"

JoystickHandler::JoystickHandler()
{
}

JoystickHandler::JoystickHandler(const char* joystick)
{
	ChangeJoystick(joystick);
}

void JoystickHandler::ChangeJoystick(const char* joystick)
{
	std::lock(fd_mutex, joystick_mutex);
	std::lock_guard<std::mutex> fd_guard(fd_mutex, std::adopt_lock);
	std::lock_guard<std::mutex> joystick_guard(joystick_mutex, std::adopt_lock);
	this->joystick = joystick;
	//TODO: open fd here
}

std::string JoystickHandler::GetJoystick() const
{
	std::lock_guard<std::mutex> guard(joystick_mutex);
	return this->joystick;
}

int JoystickHandler::GetNumAxes() const
{
	std::lock_guard<std::mutex> guard(axes_mutex);
	return axes.size();
}

int JoystickHandler::GetNumButtons() const
{
	std::lock_guard<std::mutex> guard(buttons_mutex);
	return buttons.size();
}

bool JoystickHandler::GetButtonState(int button) const
{
	std::lock_guard<std::mutex> guard(buttons_mutex);
	return buttons[button];
}

int16_t JoystickHandler::GetAxisState(int axis) const
{
	std::lock_guard<std::mutex> guard(axes_mutex);
	return axes[axis];
}

void JoystickHandler::Update()
{
	std::lock_guard<std::mutex> guard(fd_mutex);
	//update stuff here
}
