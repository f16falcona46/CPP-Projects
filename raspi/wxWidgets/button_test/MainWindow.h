#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "common.h"

class MainWindow : public wxFrame
{
public:
	MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size);
private:
	int Timer1_ticks;
	int PNG_clicks;
	wxStaticText* counterLabel;
	wxSizer* sizer;
	void OnButtonClick(wxCommandEvent& event);
	void OnPNGButtonClick(wxCommandEvent& event);
	void OnTimerTimeout(wxTimerEvent& event);

	wxDECLARE_EVENT_TABLE();
};

#endif //MAINWINDOW_H
