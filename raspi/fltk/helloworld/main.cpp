#include "precompiled_headers.h"
#include "common.h"

void CloseWindowCallback(Fl_Widget*, void* mainWindow)
{
	Fl_Window* window = (Fl_Window*)mainWindow;
	window->hide();
}

int main(int argc, char **argv)
{
	/*
	Fl_Window *window = new Fl_Window(340,180);
	window->size_range(340, 180);
	Fl_Box *box = new Fl_Box(20,40,300,100,"Hello, World!");
	window->resizable(box);
	box->box(FL_PLASTIC_UP_BOX);
	box->labelfont(FL_BOLD+FL_ITALIC);
	box->labelsize(36);
	box->labeltype(FL_SHADOW_LABEL);
	window->end();
	*/
	Fl_Window* dialog = new Fl_Double_Window(300, 300);
		Fl_Box* icon = new Fl_Box(0, 0, 50, 50, "!");
		Fl_Box* msg  = new Fl_Box(50, 10, 240, 130, "No Memory");
		msg->box(FL_PLASTIC_UP_BOX);
		Fl_Group* btns = new Fl_Group(50, 150, 250, 50);
			Fl_Button* ok = new Fl_Button(200, 150, 80, 40, "Darn!");
			ok->callback(CloseWindowCallback, dialog);
			Fl_Box* r = new Fl_Box(50, 150, 150, 50);
			r->hide();
			btns->resizable(r);
		btns->end();
		Fl_Group* radio_buttons = new Fl_Group(0, 200, 300, 100);
			Fl_Radio_Round_Button* rb1 = new Fl_Radio_Round_Button(0, 200, 300, 16, "RB1");
			Fl_Radio_Round_Button* rb2 = new Fl_Radio_Round_Button(0, 216, 300, 16, "RB2");
			Fl_Radio_Round_Button* rb3 = new Fl_Radio_Round_Button(0, 232, 300, 16, "RB3");
			Fl_Radio_Round_Button* rb4 = new Fl_Radio_Round_Button(0, 248, 300, 16, "RB4");
			r = new Fl_Box(300, 200, 0, 100);
			r->hide();
			radio_buttons->resizable(r);
		radio_buttons->end();
		radio_buttons->box(FL_ENGRAVED_FRAME);
		dialog->resizable(msg);
	dialog->end();
	dialog->show(argc, argv);
	return Fl::run();
}
