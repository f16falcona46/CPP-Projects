#include "precompiled_headers.h"
#include "common.h"

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

class MainWindow : public wxFrame
{
public:
	MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size);
private:
	wxSizer* sizer;
	wxStaticText* readingLabel;
	void OnTimerTimeout(wxTimerEvent& event);

	wxDECLARE_EVENT_TABLE();
};

#endif //MAINWINDOW_H
