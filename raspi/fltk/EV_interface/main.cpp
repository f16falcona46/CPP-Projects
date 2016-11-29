#include <cstdio>

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Group.H>
#include <FL/Enumerations.H>
#include <FL/Fl_Repeat_Button.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Radio_Light_Button.H>

struct DistancesAndWidgets {
	double Distance;
	double Offset;
	Fl_Widget* Label_distance;
	Fl_Widget* Label_offset;
	Fl_Button* Button_Tens;
	Fl_Button* Button_Units;
	Fl_Button* Button_Tenths;
	Fl_Button* Button_Hundredths;
};

double GetIncrementOfAdjustment(const DistancesAndWidgets* distances)
{
	return 10 * distances->Button_Tens->value()
		+ distances->Button_Units->value()
		+ 0.1 * distances->Button_Tenths->value()
		+ 0.01 * distances->Button_Hundredths->value();
}

void UpdateDistanceLabels(const DistancesAndWidgets* distances)
{
	char buf[20];
	std::snprintf(buf, 20, "%.2f cm", distances->Distance);
	distances->Label_distance->copy_label(buf);
	std::snprintf(buf, 20, "%.2f cm", distances->Offset);
	distances->Label_offset->copy_label(buf);
}

void Up_Dist_Callback(Fl_Widget* widget, void* data)
{
	DistancesAndWidgets* dists = static_cast<DistancesAndWidgets*>(data);
	dists->Distance += GetIncrementOfAdjustment(dists);
	UpdateDistanceLabels(dists);
}

void Down_Dist_Callback(Fl_Widget* widget, void* data)
{
	DistancesAndWidgets* dists = static_cast<DistancesAndWidgets*>(data);
	if (dists->Distance > 0) dists->Distance -= GetIncrementOfAdjustment(dists);
	UpdateDistanceLabels(dists);
}

void Up_Offset_Callback(Fl_Widget* widget, void* data)
{
	DistancesAndWidgets* dists = static_cast<DistancesAndWidgets*>(data);
	dists->Offset += GetIncrementOfAdjustment(dists);
	UpdateDistanceLabels(dists);
}

void Down_Offset_Callback(Fl_Widget* widget, void* data)
{
	DistancesAndWidgets* dists = static_cast<DistancesAndWidgets*>(data);
	if (dists->Offset > 0) dists->Offset -= GetIncrementOfAdjustment(dists);
	UpdateDistanceLabels(dists);
}

int main(int argc, char* argv[])
{
	Fl_Window* window = new Fl_Double_Window(318, 216);
		Fl_Box* dummy = new Fl_Box(0, 0, 318, 216);
		dummy->hide();
		Fl_Repeat_Button* button_dist_up = new Fl_Repeat_Button(0, 0, 90, 80, "@+92<");
		Fl_Widget* label_dist_desc = new Fl_Box(0, 80, 90, 16, "Total Distance");
		Fl_Widget* label_dist = new Fl_Box(0, 96, 90, 40);
		Fl_Repeat_Button* button_dist_down = new Fl_Repeat_Button(0, 136, 90, 80, "@+92>");
		
		Fl_Group* speed_control = new Fl_Group(95, 5, 128, 150, "Adjustment Speed");
			speed_control->box(FL_ENGRAVED_FRAME);
			speed_control->align(FL_ALIGN_TOP | FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
			Fl_Radio_Light_Button* button_tens = new Fl_Radio_Light_Button(100, 25, 118, 24, "x10");
			Fl_Radio_Light_Button* button_units = new Fl_Radio_Light_Button(100, 54, 118, 24, "x1");
			Fl_Radio_Light_Button* button_tenths = new Fl_Radio_Light_Button(100, 83, 118, 24, "x0.1");
			Fl_Radio_Light_Button* button_hundredths = new Fl_Radio_Light_Button(100, 112, 118, 24, "x0.01");
			button_tens->setonly();
		speed_control->end();
		Fl_Button* button_run = new Fl_Button(110, 160, 98, 51, "Run");
		
		Fl_Repeat_Button* button_offset_up = new Fl_Repeat_Button(228, 0, 90, 80, "@+92<");
		Fl_Widget* label_offset_desc = new Fl_Box(228, 80, 90, 16, "Lateral Offset");
		Fl_Widget* label_offset = new Fl_Box(228, 96, 90, 40);
		Fl_Repeat_Button* button_offset_down = new Fl_Repeat_Button(228, 136, 90, 80, "@+92>");
	window->end();
	window->size_range(100, 100);
	window->resizable(dummy);
	window->show(argc, argv);
	
	DistancesAndWidgets dists;
	dists.Distance = 0;
	dists.Offset = 0;
	dists.Label_distance = label_dist;
	dists.Label_offset = label_offset;
	dists.Button_Tens = button_tens;
	dists.Button_Units = button_units;
	dists.Button_Tenths = button_tenths;
	dists.Button_Hundredths = button_hundredths;
	
	button_dist_up->callback(&Up_Dist_Callback, &dists);
	button_dist_down->callback(&Down_Dist_Callback, &dists);
	button_offset_up->callback(&Up_Offset_Callback, &dists);
	button_offset_down->callback(&Down_Offset_Callback, &dists);
	
	UpdateDistanceLabels(&dists);
	
	return Fl::run();
}