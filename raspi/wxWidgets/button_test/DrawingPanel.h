#ifndef DRAWINGPANEL_H
#define DRAWINGPANEL_H

#include "common.h"

class DrawingPanel : public wxPanel
{
public:
	DrawingPanel(wxWindow* parent);
	void OnPaint(wxPaintEvent& event);
};

#endif //DRAWINGPANEL_H
