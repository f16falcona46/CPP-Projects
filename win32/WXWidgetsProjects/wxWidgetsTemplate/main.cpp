#include <wx/wxprec.h>
#include "common.h"

class TemplateApp : public wxApp
{
public:
	virtual bool OnInit() override;
};

bool TemplateApp::OnInit()
{
	wxFrame* main_window = new wxFrame(nullptr, wxID_ANY, "Button Test", wxPoint(50, 50), wxSize(450, 340));
	main_window->SetMinSize(wxSize(300, 200));
	main_window->Show(true);
	return true;
}

wxIMPLEMENT_APP(TemplateApp);
