#include "precompiled_headers.h"
#include "JoystickHandler.h"

#ifndef JS_READ_GUI_APP_H
#define JS_READ_GUI_APP_H

class JSReadGUI : public wxApp
{
public:
	virtual bool OnInit() override;
	virtual int OnExit() override;
	const JoystickHandler* GetHandler() const;
	JoystickHandler* GetHandler();
private:
	std::unique_ptr<JoystickHandler> handler;
};

wxDECLARE_APP(JSReadGUI);

#endif //JS_READ_GUI_APP_H
