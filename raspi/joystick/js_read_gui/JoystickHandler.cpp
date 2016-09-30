#include "JoystickHandler.h"
#include <exception>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/joystick.h>

JoystickHandler::JoystickHandler() : isSetUp(false), fd(-1)
{
}

JoystickHandler::JoystickHandler(const char* joystick) : isSetUp(false), fd(-1)
{
	ChangeJoystick(joystick);
}

JoystickHandler::~JoystickHandler()
{
	StopUpdateThread();
	if (fd != -1) close(fd);
}

void JoystickHandler::ChangeJoystick(const char* joystick)
{
	std::lock(fd_mutex, joystick_mutex);
	std::lock_guard<std::mutex> fd_guard(fd_mutex, std::adopt_lock);
	std::lock_guard<std::mutex> joystick_guard(joystick_mutex, std::adopt_lock);
	this->joystick = joystick;

	int newfd = open(joystick, O_RDONLY);
	if (newfd == -1) throw std::runtime_error("Couldn't open joystick.");
	if (fd != -1) close(fd);
	fd = newfd;
	std::lock_guard<std::mutex> buttons_guard(buttons_mutex);
	std::lock_guard<std::mutex> axes_guard(axes_mutex);
	
	uint8_t num_buttons;
	ioctl(fd, JSIOCGBUTTONS, &num_buttons);
	buttons.resize(num_buttons);
	uint8_t num_axes;
	ioctl(fd, JSIOCGAXES, &num_axes);
	axes.resize(num_axes);

	isSetUp = true;
}

std::string JoystickHandler::GetJoystick() const
{
	std::lock_guard<std::mutex> guard(joystick_mutex);
	if (!isSetUp) throw std::logic_error("JoystickHandler wasn't set up.");
	return this->joystick;
}

int JoystickHandler::GetNumAxes() const
{
	std::lock_guard<std::mutex> guard(axes_mutex);
	if (!isSetUp) throw std::logic_error("JoystickHandler wasn't set up.");
	return axes.size();
}

int JoystickHandler::GetNumButtons() const
{
	std::lock_guard<std::mutex> guard(buttons_mutex);
	if (!isSetUp) throw std::logic_error("JoystickHandler wasn't set up.");
	return buttons.size();
}

bool JoystickHandler::GetButtonState(int button) const
{
	std::lock_guard<std::mutex> guard(buttons_mutex);
	if (!isSetUp) throw std::logic_error("JoystickHandler wasn't set up.");
	return buttons[button];
}

int16_t JoystickHandler::GetAxisState(int axis) const
{
	std::lock_guard<std::mutex> guard(axes_mutex);
	if (!isSetUp) throw std::logic_error("JoystickHandler wasn't set up.");
	return axes[axis];
}

void JoystickHandler::Update()
{
	std::lock_guard<std::mutex> fd_guard(fd_mutex);
	js_event e;
	int result = read(fd, &e, sizeof(e));
	if (result != sizeof(e)) throw std::runtime_error("Reading from joystick failed.");
	if ((e.type & ~JS_EVENT_INIT) == JS_EVENT_BUTTON) {
		std::lock_guard<std::mutex> buttons_guard(buttons_mutex);
		buttons[e.number] = e.value;
	}
	else if ((e.type & ~JS_EVENT_INIT) == JS_EVENT_AXIS) {
		std::lock_guard<std::mutex> axes_guard(axes_mutex);
		axes[e.number] = e.value;
	}
}

void JoystickHandler::UpdateLoop()
{
	while (runUpdateThread) {
		Update();
	}
}

void JoystickHandler::StartUpdateThread()
{
	if (!isSetUp) throw std::logic_error("JoystickHandler wasn't set up.");
	runUpdateThread = true;
	updateThread = std::thread(&JoystickHandler::UpdateLoop, this);
}

void JoystickHandler::StopUpdateThread()
{
	runUpdateThread = false;
	updateThread.join();
}
