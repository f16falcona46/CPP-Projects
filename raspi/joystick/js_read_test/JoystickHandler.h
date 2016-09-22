#ifndef JOYSTICK_HANDLER_H
#define JOYSTICK_HANDLER_H

#include <cstdint>
#include <vector>
#include <mutex>
#include <string>

class JoystickHandler {
public:
	JoystickHandler();
	JoystickHandler(const char* joystick);
	void ChangeJoystick(const char* joystick);
	std::string GetJoystick() const;
	int GetNumAxes() const;
	int GetNumButtons() const;
	bool GetButtonState(int button) const;
	int16_t GetAxisState(int axis) const;
	void Update();
private:
	std::vector<bool> buttons;
	mutable std::mutex buttons_mutex;
	std::vector<int16_t> axes;
	mutable std::mutex axes_mutex;
	int fd;
	mutable std::mutex fd_mutex;
	std::string joystick;
	mutable std::string joystick_mutex;
};

#endif //JOYSTICK_HANDLER_H
