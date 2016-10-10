#ifndef JOYSTICK_HANDLER_H
#define JOYSTICK_HANDLER_H

#include <cstdint>
#include <vector>
#include <mutex>
#include <string>
#include <atomic>
#include <thread>
#include <functional>

class JoystickHandler {
public:
	JoystickHandler();
	JoystickHandler(const char* joystick);
	~JoystickHandler();
	void ChangeJoystick(const char* joystick);
	std::string GetJoystick() const;
	int GetNumAxes() const;
	int GetNumButtons() const;
	bool GetButtonState(int button) const;
	int16_t GetAxisState(int axis) const;
	void StartUpdateThread();
	void StopUpdateThread();
	void AddCallback(const std::function<void(JoystickHandler*)>& callback);
private:
	void Update();
	void UpdateLoop();
	void DoCallbacks();

	std::vector<bool> buttons;
	mutable std::mutex buttons_mutex;
	std::vector<int16_t> axes;
	mutable std::mutex axes_mutex;
	int fd;
	mutable std::mutex fd_mutex;
	std::string joystick;
	mutable std::mutex joystick_mutex;
	std::atomic<bool> isSetUp;
	std::atomic<bool> runUpdateThread;
	std::thread updateThread;
	std::vector<std::function<void(JoystickHandler*)>> callbacks;
	mutable std::mutex callbacks_mutex;
};

#endif //JOYSTICK_HANDLER_H
