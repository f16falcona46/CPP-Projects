#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Widget.H>
#include <FL/Enumerations.H>
#include <FL/Fl.H>

#include "ClockPanel.h"

int main(int argc, char **argv)
{
	Fl_Window* dialog = new Fl_Window(300, 300);
		ClockPanel* clock = new ClockPanel(0, 0, dialog->w(), dialog->h(), "Clock test");
	dialog->end();
	dialog->resizable(clock);
	dialog->size_range(300, 300);
	dialog->show(argc, argv);
	return Fl::run();
}
