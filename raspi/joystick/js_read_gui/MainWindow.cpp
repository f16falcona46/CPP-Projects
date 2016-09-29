#include "precompiled_headers.h"
#include "common.h"
#include "MainWindow.h"
#include "JSPositionPanel.h"
#include "JSReadGUI.h"

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
	EVT_TIMER(ID_Timer1, MainWindow::OnTimerTimeout)
wxEND_EVENT_TABLE()

MainWindow::MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size) :
	wxFrame(NULL, wxID_ANY, title, pos, size)
{
	wxPanel* panel = new wxPanel(this);
	wxTimer* timer = new wxTimer(this, ID_Timer1);

	wxGridBagSizer* sizer = new wxGridBagSizer();

	this->readingLabel = new wxStaticText(panel, -1, "0");
	sizer->Add(this->readingLabel, wxGBPosition(0, 0), wxGBSpan(), wxALL | wxALIGN_CENTER, 5);
	sizer->Add(new JSPositionPanel(panel), wxGBPosition(1, 0), wxGBSpan(), wxALL | wxEXPAND, 5);
	sizer->AddGrowableRow(0);
	sizer->AddGrowableRow(1);
	sizer->AddGrowableCol(0);
	sizer->AddGrowableCol(1);
	sizer->AddGrowableCol(2);

	panel->SetSizer(sizer);
	this->sizer = sizer;
	timer->Start(16);
}

void MainWindow::OnTimerTimeout(wxTimerEvent& event)
{
	const JSReadGUI& app = wxGetApp();
	this->readingLabel->SetLabelText(std::to_string(app.GetHandler()->GetAxisState(0)));
}
