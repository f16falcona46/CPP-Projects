#include "precompiled_headers.h"

class MainWindow : public wxFrame
{
public:
	MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size);

private:
	int count;
	wxStaticText* counterLabel;
	wxSizer* sizer;
	void OnButtonClick(wxCommandEvent& event);
	void OnTimerTimeout(wxTimerEvent& event);

	wxDECLARE_EVENT_TABLE();
};

class ButtonTestApp : public wxApp
{
public:
	virtual bool OnInit() override;
};

enum MainWindowIDs
{
	ID_Button1 = 1,
	ID_Button2,
	ID_Button3,
	ID_Button4,
	ID_Timer1
};

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
	EVT_BUTTON(ID_Button1, MainWindow::OnButtonClick)
	EVT_BUTTON(ID_Button2, MainWindow::OnButtonClick)
	EVT_BUTTON(ID_Button3, MainWindow::OnButtonClick)
	EVT_BUTTON(ID_Button4, MainWindow::OnButtonClick)
	EVT_TIMER(ID_Timer1, MainWindow::OnTimerTimeout)
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
	count(0)
{
	//wxPanel* panel = new wxPanel(this);
	wxTimer* timer = new wxTimer(this, ID_Timer1);

	wxGridBagSizer* sizer = new wxGridBagSizer();

	sizer->Add(new wxButton(this, ID_Button1, "Test1!!!", wxDefaultPosition, wxDefaultSize, 0), wxGBPosition(0, 0), wxGBSpan(), wxALL | wxEXPAND, 5);
	sizer->Add(new wxButton(this, ID_Button2, "Test2!!!", wxDefaultPosition, wxDefaultSize, 0), wxGBPosition(0, 1), wxGBSpan(), wxALL | wxEXPAND, 5);
	sizer->Add(new wxButton(this, ID_Button3, "Test3!!!", wxDefaultPosition, wxDefaultSize, 0), wxGBPosition(1, 0), wxGBSpan(), wxALL | wxEXPAND, 5);
	sizer->Add(new wxButton(this, ID_Button4, "Test4!!!", wxDefaultPosition, wxDefaultSize, 0), wxGBPosition(1, 1), wxGBSpan(), wxALL | wxALIGN_CENTER, 5);

	this->counterLabel = new wxStaticText(this, -1, "0");
	sizer->Add(this->counterLabel, wxGBPosition(0, 2), wxGBSpan(), wxALIGN_CENTER, 5);

	sizer->AddGrowableRow(0);
	sizer->AddGrowableRow(1);
	sizer->AddGrowableCol(0);
	sizer->AddGrowableCol(1);

	this->SetSizer(sizer);
	this->sizer = sizer;
	timer->Start(1000);
}

void MainWindow::OnButtonClick(wxCommandEvent& event)
{
	std::string text = "Test text, from button ";
	switch (event.GetId()) {
	case ID_Button1: text += "1"; break;
	case ID_Button2: text += "2"; break;
	case ID_Button3: text += "3"; break;
	case ID_Button4: text += "4"; break;
	default: text = "The button is unknown!"; break;
	}
	wxMessageBox(text.c_str(), "Test title", wxOK | wxICON_INFORMATION);
}

void MainWindow::OnTimerTimeout(wxTimerEvent& event)
{
	++this->count;
	this->counterLabel->SetLabel(std::to_string(this->count));
	sizer->Layout();
}
