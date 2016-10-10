#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>
#include <FL/Enumerations.H>

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
	Fl_Window* dialog = new Fl_Window(300, 200);
	Fl_Box* icon = new Fl_Box(0, 0, 50, 50, "!");
	Fl_Box* msg = new Fl_Box(50, 10, 240, 130, "No Memory");
	msg->box(FL_PLASTIC_UP_BOX);
	Fl_Group* btns = new Fl_Group(50, 150, 250, 50);
	Fl_Button* ok = new Fl_Button(200, 150, 80, 40, "Darn!");
	ok->callback(CloseWindowCallback, dialog);
	Fl_Box* r = new Fl_Box(50, 150, 150, 50);
	r->hide();
	btns->resizable(r);
	btns->end();
	dialog->resizable(msg);
	dialog->end();
	dialog->show(argc, argv);
	return Fl::run();
}
