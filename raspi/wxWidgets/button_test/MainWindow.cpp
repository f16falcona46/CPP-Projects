#include "precompiled_headers.h"
#include "MainWindow.h"
#include "DrawingPanel.h"

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
	EVT_BUTTON(ID_Button1, MainWindow::OnButtonClick)
	EVT_BUTTON(ID_Button2, MainWindow::OnButtonClick)
	EVT_BUTTON(ID_Button3, MainWindow::OnButtonClick)
	EVT_BUTTON(ID_Button4, MainWindow::OnButtonClick)
	EVT_TIMER(ID_Timer1, MainWindow::OnTimerTimeout)
wxEND_EVENT_TABLE()

MainWindow::MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size) :
	wxFrame(NULL, wxID_ANY, title, pos, size),
	count(0)
{
	wxPanel* panel = new wxPanel(this);
	wxTimer* timer = new wxTimer(this, ID_Timer1);

	wxGridBagSizer* sizer = new wxGridBagSizer();

	sizer->Add(new wxButton(panel, ID_Button1, "Test1!!!", wxDefaultPosition, wxDefaultSize, 0), wxGBPosition(0, 0), wxGBSpan(), wxALL | wxEXPAND, 5);
	sizer->Add(new wxButton(panel, ID_Button2, "Test2!!!", wxDefaultPosition, wxDefaultSize, 0), wxGBPosition(0, 1), wxGBSpan(), wxALL | wxEXPAND, 5);
	sizer->Add(new wxButton(panel, ID_Button3, "Test3!!!", wxDefaultPosition, wxDefaultSize, 0), wxGBPosition(1, 0), wxGBSpan(), wxALL | wxEXPAND, 5);
	sizer->Add(new wxButton(panel, ID_Button4, "Test4!!!", wxDefaultPosition, wxDefaultSize, 0), wxGBPosition(1, 1), wxGBSpan(), wxALL | wxALIGN_CENTER, 5);

	this->counterLabel = new wxStaticText(panel, -1, "0");
	sizer->Add(this->counterLabel, wxGBPosition(0, 2), wxGBSpan(), wxALL | wxALIGN_CENTER, 5);
	sizer->Add(new DrawingPanel(panel), wxGBPosition(1, 2), wxGBSpan(), wxALL | wxEXPAND, 5);
	sizer->AddGrowableRow(0);
	sizer->AddGrowableRow(1);
	sizer->AddGrowableCol(0);
	sizer->AddGrowableCol(1);
	sizer->AddGrowableCol(2);

	panel->SetSizer(sizer);
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
