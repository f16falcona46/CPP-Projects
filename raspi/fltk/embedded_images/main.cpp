#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_PNG_Image.H>

extern unsigned char _binary_BadSmiley_png_start;
extern unsigned char _binary_BadSmiley_png_end;

constexpr double frame_delay = 1.0/60;

void move_callback(void* data) {
	Fl_Widget* widget = (Fl_Widget*)data;
	int x = widget->x();
	int y = widget->y();
	widget->position(x + 1, y + 1);
	if (widget->x() + widget->w() > widget->parent()->w() || widget->y() + widget->h() > widget->parent()->h())
		widget->position(0, 0);
	widget->redraw();
	Fl::repeat_timeout(frame_delay, &move_callback, data);
}

int main(int argc, char* argv[]) {
	Fl_Window* window = new Fl_Double_Window(600, 400);
		int length = &_binary_BadSmiley_png_end - &_binary_BadSmiley_png_start;
		Fl_Image* face = new Fl_PNG_Image(nullptr, &_binary_BadSmiley_png_start, length);
		Fl_Box* box = new Fl_Box(0, 0, face->w(), face->h());
		box->image(face);
	window->end();
	window->size_range(box->w() + 50, box->h() + 50);
	window->resizable(nullptr);
	Fl::add_timeout(frame_delay, &move_callback, box);
	window->show(argc, argv);
	return Fl::run();
}