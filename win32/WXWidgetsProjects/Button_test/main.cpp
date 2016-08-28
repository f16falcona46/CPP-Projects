#include "precompiled_headers.h"
#include "common.h"
#include "MainWindow.h"

class ButtonTestApp : public wxApp
{
public:
	virtual bool OnInit() override;
};

bool ButtonTestApp::OnInit()
{
	//Initializing static things
	wxImage::AddHandler(new wxPNGHandler());
	
	MainWindow* main_window = new MainWindow("Button Test", wxPoint(50, 50), wxSize(450, 340));
	main_window->SetMinSize(wxSize(300, 200));
	main_window->Show(true);
	return true;
}

wxIMPLEMENT_APP(ButtonTestApp);
