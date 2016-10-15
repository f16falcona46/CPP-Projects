#include "ClockPanel.h"

#include <FL/fl_draw.H>
#include <FL/Fl.H>
#include <FL/Enumerations.H>

#include <chrono>
#include <ctime>
#include <iomanip>
#include <string>
#include <sstream>
#include <algorithm>
#define _USE_MATH_DEFINES
#include <cmath>

void ClockPanel::UpdateClockCB(void* userdata)
{
	ClockPanel* clock = static_cast<ClockPanel*>(userdata);
	clock->update();
	Fl::repeat_timeout(0.25, UpdateClockCB, userdata);
}

ClockPanel::ClockPanel(int x, int y, int w, int h, const char* text) : Fl_Box(x, y, w, h, text)
{
	this->box(FL_FLAT_BOX);

	Fl::add_timeout(0.25, UpdateClockCB, this);
}

ClockPanel::~ClockPanel()
{
	Fl::remove_timeout(UpdateClockCB, this);
}

void ClockPanel::draw()
{
	Fl_Box::draw();
	std::stringstream ss;
	ss << "Date and time are: " << std::put_time(std::localtime(&this->current_time), "%F %T");
	fl_color(0,0,0);
	fl_font(FL_HELVETICA, 12);
	fl_draw(ss.str().c_str(), x(), y() + fl_height() - fl_descent());
	struct std::tm* tm;
	tm = localtime(&this->current_time);
	if (tm == nullptr) throw std::runtime_error("Couldn't extract time.");
	fl_arc(x(), y(), w(), h(), 0, 360);
	int hand_length = std::min(w(), h())/2;
	int sec_from_12h = 3600*(tm->tm_hour%12) + 60*tm->tm_min + tm->tm_sec;
	double minutes = sec_from_12h/60.0;
	double hours = minutes/60.0;
	fl_color(FL_BLACK);
	fl_line_style(FL_SOLID, 5);
	fl_line(x() + w()/2,
		y() + h()/2,
		x() + w()/2 + static_cast<int>(hand_length*4/5*std::sin(-2*minutes*M_PI/60 + M_PI)),
		y() + h()/2 + static_cast<int>(hand_length*4/5*std::cos(-2*minutes*M_PI/60 + M_PI)));
	fl_line(x() + w()/2,
		y() + h()/2,
		x() + w()/2 + static_cast<int>(hand_length/3*std::sin(-2*hours*M_PI/12 + M_PI)),
		y() + h()/2 + static_cast<int>(hand_length/3*std::cos(-2*hours*M_PI/12 + M_PI)));
	fl_color(FL_RED);
	fl_line_style(FL_SOLID, 5);
	fl_line(x() + w()/2,
		y() + h()/2,
		x() + w()/2 + static_cast<int>(hand_length*std::sin(-2*tm->tm_sec*M_PI/60 + M_PI)),
		y() + h()/2 + static_cast<int>(hand_length*std::cos(-2*tm->tm_sec*M_PI/60 + M_PI)));
}

void ClockPanel::update()
{
	this->current_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	this->redraw();
}
