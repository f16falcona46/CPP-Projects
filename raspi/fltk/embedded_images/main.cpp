#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_PNG_Image.H>

extern unsigned char _binary_BadSmiley_png_start;
extern unsigned char _binary_BadSmiley_png_end;


int main(int argc, char* argv[]) {
	Fl_Window* window = new Fl_Double_Window(400, 300);
		Fl_Box* box = new Fl_Box(0, 0, 400, 300);
		int length = &_binary_BadSmiley_png_end - &_binary_BadSmiley_png_start;
		box->image(new Fl_PNG_Image(nullptr, &_binary_BadSmiley_png_start, length));
	window->end();
	window->resizable(box);
	window->show(argc, argv);
	return Fl::run();
}