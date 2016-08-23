#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

#endif //MAINWINDOW_H
