#include <wx/wxprec.h>
#include <wx/gbsizer.h>
#include "common.h"
#include "MainWindow.h"
#include "DrawingPanel.h"

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
	EVT_BUTTON(ID_Button1, MainWindow::OnButtonClick)
	EVT_BUTTON(ID_Button2, MainWindow::OnButtonClick)
	EVT_BUTTON(ID_Button3, MainWindow::OnButtonClick)
	EVT_BUTTON(ID_Button4, MainWindow::OnPNGButtonClick)
	EVT_TIMER(ID_Timer1, MainWindow::OnTimerTimeout)
wxEND_EVENT_TABLE()

MainWindow::MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size) :
	wxFrame(NULL, wxID_ANY, title, pos, size),
	Timer1_ticks(0),
	PNG_clicks(0)
{
	wxPanel* panel = new wxPanel(this);
	wxTimer* timer = new wxTimer(this, ID_Timer1);

	wxGridBagSizer* sizer = new wxGridBagSizer();

	sizer->Add(new wxButton(panel, ID_Button1, "Test1!!!", wxDefaultPosition, wxDefaultSize, 0), wxGBPosition(0, 0), wxGBSpan(), wxALL | wxEXPAND, 5);
	sizer->Add(new wxButton(panel, ID_Button2, "Test2!!!", wxDefaultPosition, wxDefaultSize, 0), wxGBPosition(0, 1), wxGBSpan(), wxALL | wxEXPAND, 5);
	sizer->Add(new wxButton(panel, ID_Button3, "Test3!!!", wxDefaultPosition, wxDefaultSize, 0), wxGBPosition(1, 0), wxGBSpan(), wxALL | wxEXPAND, 5);
	sizer->Add(new wxButton(panel, ID_Button4, "Make a PNG!!!", wxDefaultPosition, wxDefaultSize, 0), wxGBPosition(1, 1), wxGBSpan(), wxALL | wxALIGN_CENTER, 5);

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
	default: wxFAIL_MSG("Event ID unknown!"); break;
	}
	wxPrintf("%s\n", text);
	wxMessageBox(text.c_str(), "Test title", wxOK | wxICON_INFORMATION);
}

void MainWindow::OnPNGButtonClick(wxCommandEvent& event)
{
	++this->PNG_clicks;
	const int width = 500;
	const int height = 500;
	const double scale = 50/this->PNG_clicks;
	wxBitmap bitmap(width, height);
	wxMemoryDC dc;
	dc.SelectObject(bitmap);
	dc.SetBackground(*wxBLACK_BRUSH);
	dc.Clear();
	dc.SetPen(*wxGREEN_PEN);
	for (int i = 0; i <= width; i += 40) {
		dc.DrawLine(0, i, width, i);
		dc.DrawLine(i, 0, i, height);
	}
	dc.SetPen(*wxWHITE_PEN);
	for (int i = 0; i <= width - 5; i += 5) {
		dc.DrawLine(i, -scale * std::sin(i / scale) + height / 2, i + 5, -scale * std::sin((i + 5) / scale) + height / 2);
	}
	dc.SelectObject(wxNullBitmap);
	std::wstring filename = L"garbage";
	filename += std::to_wstring(this->PNG_clicks);
	filename += L".png";
	bitmap.SaveFile(filename, wxBITMAP_TYPE_PNG, nullptr);
}

void MainWindow::OnTimerTimeout(wxTimerEvent& event)
{
	++this->Timer1_ticks;
	this->counterLabel->SetLabel(std::to_wstring(this->Timer1_ticks));
	sizer->Layout();
}
