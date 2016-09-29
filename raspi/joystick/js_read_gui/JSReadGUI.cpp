#include "precompiled_headers.h"
#include "common.h"
#include "JSReadGUI.h"
#include "MainWindow.h"

bool JSReadGUI::OnInit()
{
	handler.reset(new JoystickHandler("/dev/input/js0"));
	handler->StartUpdateThread();
	MainWindow* main_window = new MainWindow("Joystick Reading", wxPoint(50, 50), wxSize(450, 340));
	main_window->SetMinSize(wxSize(300, 200));
	main_window->Show(true);
	return true;
}

int JSReadGUI::OnExit()
{
	handler->StopUpdateThread();
	handler.reset(nullptr);
	return 0;
}

const JoystickHandler* JSReadGUI::GetHandler() const
{
	return this->handler.get();
}

JoystickHandler* JSReadGUI::GetHandler()
{
	return this->handler.get();
}

wxIMPLEMENT_APP(JSReadGUI);
