#ifndef CLOCK_PANEL_H
#define CLOCK_PANEL_H

#include <FL/Fl_Box.H>
#include <ctime>

class ClockPanel : public Fl_Box {
public:
	ClockPanel(int x, int y, int w, int h, const char* text = nullptr);
	virtual ~ClockPanel();
	virtual void draw() override;
private:
	void update();
	std::time_t current_time;
	static void UpdateClockCB(void* userdata);
};

#endif //CLOCK_PANEL_H
