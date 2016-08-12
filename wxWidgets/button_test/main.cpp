#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <memory>

class MainWindow : public wxFrame
{
public:
	MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size);

private:
	void OnButtonClick(wxCommandEvent& event);
	
	wxButton* test_button;

	wxDECLARE_EVENT_TABLE();
};

class ButtonTestApp : public wxApp
{
public:
	virtual bool OnInit() override;
};

enum MainWindowControls
{
	ID_Button = 1
};

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
	EVT_BUTTON(ID_Button, MainWindow::OnButtonClick)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(ButtonTestApp);

bool ButtonTestApp::OnInit()
{
	MainWindow* main_window = new MainWindow("Button Test", wxPoint(50, 50), wxSize(450, 340));
	main_window->SetMinSize(wxSize(300, 200));
	main_window->Show(true);
	return true;
}

MainWindow::MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size) :
	wxFrame(NULL, wxID_ANY, title, pos, size),
	test_button(new wxButton(this, ID_Button, "Test!!!", wxDefaultPosition, wxDefaultSize, 0))
{
}

void MainWindow::OnButtonClick(wxCommandEvent& event)
{
	wxMessageBox("Test text", "Test title", wxOK | wxICON_INFORMATION);
}
